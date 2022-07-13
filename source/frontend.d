module frontend;
import logger;
import utils;
import utils.stack;
import grammar.scot;
import errors;
import ast;

@safe:

private struct Scope {
}

private string idxIfName(ref const(ParseTree) child, size_t idx, const string name) {
    assert(child.children[idx].name == name, child.children[idx].name ~ " != " ~ name);
    return child.matches[idx];
}

class Frontend {

    private Logger log;
    private string m_input;

    private ParseTree m_cst;
    private Program m_program;
    private Stack!Scope m_scope;

    public ref ParseTree cst() {
        return m_cst;
    }

    this(Logger logger, string input) {
        log = logger;
        m_input = input;
        m_cst = Scotch(input);
    }

    Program parse() {
        parseImpl(m_cst);
        return m_program;
    }

    private void addImports(const ref ParseTree cst) {
        log.infoln(cst.matches);
    }

    private void addGlobalData(const ref ParseTree cst) {
        log.infoln(cst.matches);
    }

    private void parseImpl(const ref ParseTree cst) {
        if (!cst.successful) {
            log.error(cst);
            throw new FrontendException("cst failed");
        }
        foreach (ref child; cst.children) {
            switch (child.name) {
                case "Scotch.Module":
                    parseImpl(child);
                    break;
                case "Scotch.ModuleDeclaration":
                    m_program.modName = child.idxIfName(0, "Scotch.BareID");
                    break;
                case "Scotch.ImportBlock":
                    addImports(child);
                    break;
                case "Scotch.GlobalData":
                    addGlobalData(child);
                    break;
                case "Scotch.FunctionDefinition":
                    m_program.funcs ~= Function(child.idxIfName(0, "Scotch.BareID"));
                    log.infoln(child.matches[1 .. $]);
                    break;
                case "Scotch.ParameterList":
                case "Scotch.Parameter":
                case "Scotch.TypeAnnotation":
                case "Scotch.ErrAnnotation":
                case "Scotch.ExpressionStatement":
                case "Scotch.Expression":
                case "Scotch.PrimaryExpression":
                case "Scotch.PostfixExpression":
                case "Scotch.UnaryExpression":
                case "Scotch.UnaryOperator":
                case "Scotch.ExponentExpression":
                case "Scotch.TimesExpression":
                case "Scotch.PlusExpression":
                case "Scotch.RelationalExpression":
                case "Scotch.EqualityExpression":
                case "Scotch.LogicalANDExpression":
                case "Scotch.LogicalORExpression":
                case "Scotch.LowestPriorityOpExpression":
                case "Scotch.VariableDefinition":
                case "Scotch.VariableDeclaration":
                case "Scotch.Mod":
                case "Scotch.ControlFlow":
                case "Scotch.Return":
                case "Scotch.Fail":
                case "Scotch.Break":
                case "Scotch.Continue":
                case "Scotch.Conditionals":
                case "Scotch.IfExpression":
                case "Scotch.CaseExpression":
                case "Scotch.CaseVariantList":
                case "Scotch.CaseVariant":
                case "Scotch.CaseVariantValue":
                case "Scotch.CaseVariantPrimaryValue":
                case "Scotch.CaseVariantAndValue":
                case "Scotch.CaseVariantOrValue":
                case "Scotch.CaseVariantName":
                case "Scotch.TOKEN":
                case "Scotch.MODULE":
                case "Scotch.IMPORT":
                case "Scotch.DEF":
                case "Scotch.ERR":
                case "Scotch.RETURN":
                case "Scotch.FAIL":
                case "Scotch.BREAK":
                case "Scotch.CONTINUE":
                case "Scotch.STATIC":
                case "Scotch.CONST":
                case "Scotch.LET":
                case "Scotch.TRUE":
                case "Scotch.FALSE":
                case "Scotch.IF":
                case "Scotch.ELSE":
                case "Scotch.CASE":
                case "Scotch.Underscore":
                case "Scotch.SEP":
                case "Scotch.OPERATOR":
                case "Scotch.MATH_OPERATOR":
                case "Scotch.COMP_OPERATOR":
                case "Scotch.LOGIC_OPERATOR":
                case "Scotch.CASE_NOT":
                case "Scotch.CASE_AND":
                case "Scotch.CASE_OR":
                case "Scotch.NOT":
                case "Scotch.POWER":
                case "Scotch.ROOT":
                case "Scotch.LOG":
                case "Scotch.TIMES":
                case "Scotch.DIVIDE":
                case "Scotch.MODULO":
                case "Scotch.PLUS":
                case "Scotch.MINUS":
                case "Scotch.LESS_EQ":
                case "Scotch.LESS":
                case "Scotch.GREATER_EQ":
                case "Scotch.GREATER":
                case "Scotch.EQUAL":
                case "Scotch.NOT_EQUAL":
                case "Scotch.AND":
                case "Scotch.OR":
                case "Scotch.Literal":
                case "Scotch.StringLiteral":
                case "Scotch.CharLiteral":
                case "Scotch.Int32Literal":
                case "Scotch.Int64Literal":
                case "Scotch.Float32Literal":
                case "Scotch.Float64Literal":
                case "Scotch.BoolLiteral":
                case "Scotch.LambdaLiteral":
                case "Scotch.LambdaArgs":
                case "Scotch.SectionLiteral":
                case "Scotch.DQChar":
                case "Scotch.EscapeSequence":
                case "Scotch.Integer":
                case "Scotch.Sign":
                case "Scotch.Spacing":
                case "Scotch.Comment":
                    throw new InternalError(child.name ~ " not handled yet");
                case "Scotch.WillNotMatch":
                    throw new InternalError(
                        "Parsed `Scotch.WillNotMatch` suggesting a significant error in the grammar"
                    );
                case "Scotch.NamespacedID":
                case "Scotch.BareID":
                case "Scotch.ImportList":
                    throw new InternalError(child.name ~ " should not be parsed on it's own");
                default:
                    throw new InternalError("unknown parse tree component " ~ child.name);
            }
        }
    }
}
