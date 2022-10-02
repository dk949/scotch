module frontend;

@safe:

import ast;
import errors;
import frontend.utils;
import grammar.scot.rules;
import grammar.scot;
import logger;
import std.algorithm;
import std.range;
import utils;

class Frontend {

    private Logger log;
    private string m_input;

    private ParseTree m_cst;
    private Program m_program;
    private Scope* m_currentScope;

    public ref ParseTree cst() {
        return m_cst;
    }

    this(Logger logger, string input) {
        log = logger;
        m_input = input;
        m_cst = Scotch(input);
        m_currentScope = new Scope(null); // Global scope
    }

    private void addImports(ref const(ParseTree) cst)
    in (cst.successful) {
        placeholder();
    }

    private void addGlobalData(ref const(ParseTree) cst)
    in (cst.successful) {
        placeholder();
    }

    private Var* addVariable(ref const(ParseTree) cst)
    in (cst.successful)
    out (r; r !is null) {
        Mod mod = cst[0].toMod;
        string name = cst.getMatchAt(1, Rule.BareID);

        if (cst.length == 3)
            m_currentScope.variables[name] = Var(mod, parseTypeAnnotation(cst[2]), m_currentScope);
        else if (cst.length == 2)
            m_currentScope.variables[name] = Var(mod, unknownType, m_currentScope);
        else
            unreachable("expected expression to have 2 or 3 children");

        /*
         * NOTE: as far as I can tell, this is valid and a rehash will not invalidate the pointger
         * see https://forum.dlang.org/thread/mpwtzpldeqmacbjbzsuo@forum.dlang.org
         * > The whole AA documentation probably needs some attention.
         *  -Steve
         */
        return name in m_currentScope.variables;
    }

    private NamespacedID parseTypeAnnotation(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.TypeAnnotation);
        cst.assertNumChild(1);
    }
    out (r; r.id !is null) {
        return cst.getChildAt(0, Rule.NamespacedID).toNamespacedID;
    }

    private void parseParamList(ref const(ParseTree) cst)
    in (cst.successful) {
        foreach (param; cst) {
            param.assertType(Rule.Parameter);
            m_program.funcs.back.args[param.getMatchAt(0, Rule.BareID)] =
                parseTypeAnnotation(param.getChildAt(1, Rule.TypeAnnotation));
        }
    }

    private void parseReturnType(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.TypeAnnotation);
    }
    do {
        m_program.funcs.back.ret = cst
            .getChildAt(0, Rule.NamespacedID)
            .toNamespacedID;
    }

    private void parseErrType(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.ErrAnnotation);
    }
    do {
        m_program.funcs.back.err = cst
            .getChildAt(0, Rule.NamespacedID)
            .toNamespacedID;
    }

    private void parseFunc(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.FunctionDefinition);
    }
    do {
        size_t childNum = 0;
        m_program.funcs ~= Function(cst.getMatchAt(childNum, Rule.BareID));
        childNum++;

        if (cst[childNum].name == Rule.ParameterList) {
            parseParamList(cst.getChildAt(childNum, Rule.ParameterList));
            childNum++;
        }
        if (cst[childNum].name == Rule.TypeAnnotation) {
            parseReturnType(cst.getChildAt(childNum, Rule.TypeAnnotation));
            childNum++;
        }
        if (cst[childNum].name == Rule.ErrAnnotation) {
            parseErrType(cst.getChildAt(childNum, Rule.ErrAnnotation));
            childNum++;
        }
        m_program.funcs.back.val = parseExpression(cst.getChildAt(childNum, Rule.Expression));
        childNum++;
        assertF(cst.length == childNum, "expected to parse all children");
    }

    private Expr parseExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Expression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        switch (cst[0].name) {
            case Rule.LowestPriorityOpExpression:
                return parseLowestPriorityOpExpression(
                    cst.getChildAt(0, Rule.LowestPriorityOpExpression)
                );
            case Rule.VariableDefinition:
                return parseVariableDefinition(cst[0]);
            case Rule.ControlFlow:
                return parseControlFlow(cst[0]);
            case Rule.Conditionals:
                return parseConditional(cst[0]);
            case Rule.CatchExpression:
            default:
                cannotParse(cst[0].name);
        }
    }

    private Expr parseConditional(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Conditionals);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        switch (cst[0].name) {
            case Rule.IfExpression:
                return parseIf(cst[0]);
            case Rule.CaseExpression:
                return parseCase(cst[0]);
            default:
                cannotParse(cst[0].name);
        }
    }

    private Expr parseIf()(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.IfExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        log.infoln(cst);
        notImplemented;
    }

    private Expr parseCase()(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.CaseExpression);
        cst.assertNumChild(2);
    }
    out (r; r !is null) {
        // Case introduces a new scope (?)
        m_currentScope = new Scope(m_currentScope);
        auto case_ = new Case(parseExpression(cst[0]),parseCaseVariants(cst[1]));
        m_currentScope = m_currentScope.outer;
        return case_;
    }

    private CaseVariant[] parseCaseVariants(ref const(ParseTree) cst)
    in (cst.successful)
    out (r; r !is null && r.all) {
        auto a = appender!(CaseVariant[]);
        foreach (c; cst)
            a.put(parseCaseVariant(c));
        return a.data;
    }

    private CaseVariant parseCaseVariant(ref const(ParseTree) cst)
    in (cst.successful)
    out (r; r !is null) {
        size_t childNum = 0;
        Var* var = null;
        if (cst[childNum].name == Rule.CaseVariantName) {
            string name = cst[childNum].getMatchAt(1, Rule.BareID);
            if (cst[childNum].length == 3)
                m_currentScope.variables[name] = Var(Mod.CONST, parseTypeAnnotation(
                        cst[childNum][2]), m_currentScope);
            else
                m_currentScope.variables[name] = Var(Mod.CONST, unknownType, m_currentScope);

            var = name in m_currentScope.variables;
            childNum++;
        }
        auto val = parseCaseValue(cst[childNum]);
        childNum++;

        auto expr = parseExpression(cst[childNum]);
        childNum++;

        assertF(cst.length == childNum, "expected to parse all children");
        return new CaseVariant(var, val, expr);
    }

    private Expr parseCaseValue(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.CaseVariantValue);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
                return parseCaseOrValue(cst[0]);
    }

    private Expr parseCaseOrValue(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.CaseVariantOrValue);
    }
    out (r; r !is null) {
        switch (cst.length) {
            case 1:
                return parseCaseAndValue(cst[0]);
            case 2:
            case 3:
                placeholder("cannot parse case OR expression");
                return new Unsupported();
            default:
                unexpectedNumChildren(cst.length);
        }
    }

    private Expr parseCaseAndValue(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.CaseVariantAndValue);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseCasePrimaryValue(cst[0]);
    }

    private Expr parseCasePrimaryValue(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.CaseVariantPrimaryValue);
        cst.assertNumChild(1);
    }

    out (r; r !is null) {
        size_t childNum;
        bool not = false;
        if (cst[childNum].name == Rule.CASE_NOT) {
            not = true;
            childNum++;
        }
        switch (cst[childNum].name) {
            case Rule.LowestPriorityOpExpression:
                return parseLowestPriorityOpExpression(cst[childNum]);
            case Rule.CaseVariantValue:
                return parseCaseValue(cst[childNum]);
            default:
                cannotParse(cst[childNum].name);
        }
    }

    private Expr parseControlFlow(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.ControlFlow);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        switch (cst[0].name) {
            case Rule.Return:
                return parseReturn(cst[0]);
            case Rule.Fail:
                return parseFail(cst[0]);
            case Rule.Break:
                return parseBreak(cst[0]);
            case Rule.Continue:
                return parseContinue(cst[0]);
            default:
                cannotParse(cst[0].name);
        }
    }

    private Expr parseReturn(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Return);
    }
    out (r; r !is null) {
        switch (cst.length) {
            case 1:
                return new Return(new EmptyExpr());
            case 2:
                return new Return(parseExpression(cst[1]));
            default:
                unexpectedNumChildren(cst.length);
        }
    }

    private Expr parseFail(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Fail);
    }
    out (r; r !is null) {
        switch (cst.length) {
            case 1:
                return new Fail(new EmptyExpr());
            case 2:
                return new Fail(parseExpression(cst[1]));
            default:
                unexpectedNumChildren(cst.length);
        }
    }

    private Expr parseBreak(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Break);
    }
    out (r; r !is null) {
        switch (cst.length) {
            case 1:
                return new Break(new EmptyExpr());
            case 2:
                return new Break(parseExpression(cst[1]));
            default:
                unexpectedNumChildren(cst.length);
        }
    }

    private Expr parseContinue(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.Continue);
        cst.assertNumChild(1);
        cst[0].assertType(Rule.CONTINUE);
    }
    out (r; r !is null) {
        return new Continue();
    }

    private Expr parseVariableDefinition(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.VariableDefinition);
        cst.assertNumChild(2);
    }
    out (r; r !is null) {
        return new VarDecl(addVariable(cst[0]), parseExpression(cst[1]));
    }

    private Expr parseLowestPriorityOpExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.LowestPriorityOpExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseLogicalORExpression(cst.getChildAt(0, Rule.LogicalORExpression));
    }

    private Expr parseLogicalORExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.LogicalORExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseLogicalANDExpression(cst.getChildAt(0, Rule.LogicalANDExpression));
    }

    private Expr parseLogicalANDExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.LogicalANDExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseEqualityExpression(cst.getChildAt(0, Rule.EqualityExpression));
    }

    private Expr parseEqualityExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.EqualityExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseRelationalExpression(cst.getChildAt(0, Rule.RelationalExpression));
    }

    private Expr parseRelationalExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.RelationalExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parsePlusExpression(cst.getChildAt(0, Rule.PlusExpression));
    }

    private Expr parsePlusExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.PlusExpression);
    }
    out (r; r !is null) {
        if (cst.length != 1) {
            placeholder;
            return new Unsupported();
        }
        return parseTimesExpression(cst.getChildAt(0, Rule.TimesExpression));
    }

    private Expr parseTimesExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.TimesExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseExponentExpression(cst.getChildAt(0, Rule.ExponentExpression));
    }

    private Expr parseExponentExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.ExponentExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseUnaryExpression(cst.getChildAt(0, Rule.UnaryExpression));
    }

    private Expr parseUnaryExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.UnaryExpression);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parsePostfixExpression(cst.getChildAt(0, Rule.PostfixExpression));
    }

    private Expr parsePostfixExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.PostfixExpression);
    }
    out (r; r !is null) {
        switch (cst.length) {
            case 1:
                return parsePrimaryExpression(cst.getChildAt(0, Rule.PrimaryExpression));
            case 2:
                placeholder("can't parse postfix expr");
                return new Unsupported();
            default:
                unexpectedNumChildren(cst.length);
        }
    }

    private Expr parsePrimaryExpression(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.PrimaryExpression);
    }
    out (r; r !is null) {
        switch (cst[0].name) {
            case Rule.ExpressionStatement: {
                // Start of new scope
                m_currentScope = new Scope(m_currentScope);
                auto cs = new CompoundExpr();
                foreach (stmt; cst) {
                    cs.statements ~= parseExpressionStatement(stmt);
                }
                // end of scope
                m_currentScope = m_currentScope.outer;
                return cs;
            }
            case Rule.Literal:
                cst.assertNumChild(1);
                return parseLiteral(cst[0]);
            case Rule.Expression:
            case Rule.NamespacedID:
            case Rule.EmptyExpression:
                return new EmptyExpr();
            default:
                cannotParse(cst[0].name);
        }
    }

    private Expr parseLiteral(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertNumChild(1);
        cst.assertType(Rule.Literal);

        cst[0].assertSuccess;
    }
    out (r; r !is null) {
        switch (cst[0].name) {
            case Rule.BoolLiteral:
                cst[0][0].assertSuccess;
                switch (cst[0][0].name) {
                    case Rule.TRUE:
                        return new BoolLiteral(true);
                    case Rule.FALSE:
                        return new BoolLiteral(false);
                    default:
                        unreachable("invalid bool value " ~ cst[0][0].name);
                }
            case Rule.Int32Literal:
                return parseInt!(32)(cst[0]);
            case Rule.Int64Literal:
                return parseInt!(64)(cst[0]);
            case Rule.Float32Literal:
                return parseFloat!(32)(cst[0]);
            case Rule.Float64Literal:
                return parseFloat!(64)(cst[0]);
            case Rule.CharLiteral:
                return parseCharLiteral(cst[0]);
            case Rule.StringLiteral:
                return parseStringLiteral(cst[0]);
            case Rule.LambdaLiteral:
                placeholder("Lambda literals not supported");
                return new Unsupported();
            case Rule.SectionLiteral:
                placeholder("Section literals not supported");
                return new Unsupported();
            default:
                cannotParse(cst[0].name);
        }
    }

    private Expr parseInt(int size)(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertNumMatches(1);
    }
    out (r; r !is null) {
        import std.conv;

        string i = cst.matches[0];
        switch (i[$ - 1]) {
            case 'i':
            case 'l':
                i.length--;
                break;
            default:
                break;
        }
        static if (size == 32)
            return new IntLiteral(i.to!int);
        else static if (size == 64)
            return new IntLiteral(i.to!long);
        else
            static assert(0, "invalid int size");

    }

    private Expr parseFloat(int size)(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertNumMatches(1);
    }
    out (r; r !is null) {
        import std.conv;

        string f = cst.matches[0];
        switch (f[$ - 1]) {
            case 'f':
            case 'd':
                f.length--;
                break;
            default:
                break;
        }
        static if (size == 32)
            return new FloatLiteral(f.to!float);
        else static if (size == 64)
            return new FloatLiteral(f.to!double);
        else
            static assert(0, "invalid float size");

    }

    private Expr parseCharLiteral()(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertNumMatches(1);
    }
    out (r; r !is null) {
        string ch = cst.matches[0];
        if (ch.length == 1)
            return new CharLiteral(cst.matches[0][0]);
        else if (ch.length == 2 && ch[0] == '\\') {
            switch (ch[1]) {
                case '0':
                    return new CharLiteral('\0');
                case 'a':
                    return new CharLiteral('\a');
                case 'b':
                    return new CharLiteral('\b');
                case 't':
                    return new CharLiteral('\t');
                case 'n':
                    return new CharLiteral('\n');
                case 'v':
                    return new CharLiteral('\v');
                case 'f':
                    return new CharLiteral('\f');
                case 'r':
                    return new CharLiteral('\r');
                default:
                    return new CharLiteral(ch[1]);
            }
        } else {
            unreachable("multichar char has more than 2 characters or first char is not \\");
        }
    }

    private Expr parseStringLiteral()(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertNumMatches(1);
    }
    out (r; r !is null) {
        return new StringLiteral(cst.matches[0]);
    }

    private Expr parseExpressionStatement(ref const(ParseTree) cst)
    in {
        cst.assertSuccess;
        cst.assertType(Rule.ExpressionStatement);
        cst.assertNumChild(1);
    }
    out (r; r !is null) {
        return parseExpression(cst[0]);
    }

    private void parseModule(ref const(ParseTree) cst)
    in (cst.successful) {
        size_t childNum = 0;
        m_program.modName = cst
            .getChildAt(childNum, Rule.ModuleDeclaration)
            .getMatchAt(childNum, Rule.BareID);

        cst.getChildAt(childNum, Rule.ModuleDeclaration).assertNumChild(1);

        for (childNum = 1; cst.length > childNum && cst[childNum].name == Rule
            .ImportBlock; childNum++) {
            addImports(cst.getChildAt(childNum, Rule.ImportBlock));
        }

        for (; cst.length > childNum && cst[childNum].name == Rule.GlobalData; childNum++) {
            addGlobalData(cst.getChildAt(childNum, Rule.GlobalData));
        }

        for (; cst.length > childNum && cst[childNum].name == Rule.FunctionDefinition;
            childNum++) {
            parseFunc(cst.getChildAt(childNum, Rule.FunctionDefinition));
        }
        assertF(cst.length == childNum, "expected to parse all children");
    }

    Program parse() {
        if (!m_cst.successful)
            throw new FrontendException(m_cst.failMsg());

        m_cst.assertNumChild(1);
        parseModule(m_cst.getChildAt(0, Rule.Module));
        return m_program;
    }
}
