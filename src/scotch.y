%language "c++"
%locations
%define api.value.type variant
%define api.namespace {scotch}
%define parse.trace

%defines "parser.hpp"
%output "parser.cpp"

%code top {
#include "ast.hpp"
#include "ast_compiler.hpp"
#include "parser_options.hpp"
#include <iostream>
#include "type_store.hpp"

TypeStore ts;
}

%code requires{
#include <memory>
#include <vector>
#include "ast.hpp"
}

%code provides {
int yylex(scotch::parser::semantic_type* yylval, scotch::parser::location_type* yylloc);

}


%token MODULE
%token DEF
%token CONST
%token LET
%token RETURN
%token LPAREN
%token RPAREN
%token LCURLY
%token RCURLY
%token IF
%token ELSE
%token COLON
%token SEMICOLON
%token COMMA
%token ASSIGN
%token <std::string> IDENT
%token <long long> Int64
%token <long long> Int32
%token <double> Float32
%token <double> Float64
%token <bool> TRUE
%token <bool> FALSE


%left EQUAL
%left PLUS MINUS
%left TIMES DIVIDE MODULO
%left LESS GREATER LESSTHENEQUAL GREATERTHENEQUAL
%left OR
%left AND
%precedence NOT

%type<Arg>                                  arg
%type<std::vector<Arg>>                     args
%type<std::shared_ptr<Expr>>                binExpr
%type<std::shared_ptr<Declare>>             declaration
%type<std::shared_ptr<Assign>>              assignment
%type<std::shared_ptr<Expr>>                expression
%type<FunctionDef>                          funcdef
%type<std::vector<FunctionDef>>             funcdefs
%type<Ident>                                ident
%type<Mod>                                  mod
%type<Module>                               module
%type<Program>                              program
%type<std::shared_ptr<If>>                  if
%type<std::vector<std::shared_ptr<Expr>>>   else
%type<std::shared_ptr<Condition>>           condition
%type<std::shared_ptr<Return>>              return
%type<std::shared_ptr<Expr>>                statement
%type<std::vector<std::shared_ptr<Expr>>>   block
%type<std::vector<std::shared_ptr<Expr>>>   statements
%type<std::shared_ptr<Expr>>                valExpr

%%
start       : program
            {
                auto program = $1;

                auto pipeline = ParserOptions::takePipeline();
                pipeline.run(program, ts);
            }
            ;

program     : module { $$ = Program{$1}; }
            ;

module      : MODULE ident SEMICOLON funcdefs { $$ = Module{$2, $4};}
            ;

funcdefs    : { $$ = std::vector<FunctionDef>{}; }
            | funcdefs funcdef { $1.push_back($2); $$ = $1; }
            ;

funcdef     : DEF ident LPAREN args RPAREN COLON ident block { $$ = FunctionDef{$2, ts.makeType(($7).name()), $4, $8}; }
            ;

block       : LCURLY statements RCURLY { $$ = $2;}
            ;

args        : { $$ = std::vector<Arg>{}; }
            | arg { $$ = std::vector<Arg>{$1}; }
            | args COMMA arg { $1.push_back($3); $$ = $1; }
            ;

arg         : ident COLON ident { $$ = Arg($1, ts.makeType(($3).name())); }
            ;

statements  : { $$ = std::vector<std::shared_ptr<Expr>>{}; }
            | statements statement { $1.push_back($2); $$ = $1; }
            ;

statement   : SEMICOLON {$$ = std::make_shared<EmptyExpr>();}
            | declaration SEMICOLON { $$ = $1;}
            | assignment SEMICOLON { $$ = $1;}
            | return SEMICOLON {$$ = $1;}
            | condition {$$ = $1;}
            | expression SEMICOLON {$$ = $1;}
            ;

declaration : mod ident COLON ident ASSIGN expression { $$ = std::make_shared<Declare>($1, $2, ts.makeType($4.name()), $6);}
            | mod ident ASSIGN expression { $$ = std::make_shared<Declare>($1, $2, std::nullopt, $4);}
            ;

assignment  : ident ASSIGN expression { $$ = std::make_shared<Assign>($1, $3);}
            ;

return      : RETURN expression { $$ = std::make_shared<Return>($2); }
            ;

condition   : if { $$ = static_cast<std::shared_ptr<Condition>>($1); }
            | if else { $$ = static_cast<std::shared_ptr<Condition>>(std::make_shared<IfElse>(*($1.get()), $2)); }
            ;

if          : IF LPAREN expression RPAREN block { $$ = std::make_shared<If>($3, $5); }
            ;

else        : ELSE block { $$ = $2; }
            ;

expression  : binExpr
            | valExpr
            ;

binExpr     : expression PLUS expression { $$ = std::make_shared<Add>($1, $3);}
            ;

valExpr     : ident     { $$ = std::make_shared<VarExpr>($1);}
            | TRUE      { $$ = std::make_shared<Bool>(true);  }
            | FALSE     { $$ = std::make_shared<Bool>(false);  }
            | Int32     { $$ = std::make_shared<Int32>($1);  }
            | Int64     { $$ = std::make_shared<Int64>($1);  }
            | Float32   { $$ = std::make_shared<Float32>($1);  }
            | Float64   { $$ = std::make_shared<Float64>($1);  }
            ;


mod         : CONST { $$ = Mod::CONST;}
            | LET { $$ = Mod::LET; }
            ;

ident       : IDENT { $$ = Ident{$1}; }
            ;

%%
