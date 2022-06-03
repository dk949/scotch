#ifndef FWD_HPP
#define FWD_HPP
// args.hpp:
enum class Verbosity;
struct Args;

// ast.hpp:
class Add;
class Assign;
class BuiltInLiteral;
class Declare;
class EmptyExpr;
class Expr;
class Float32;
class Float64;
class FunctionDef;
class Ident;
class Int32;
class Int64;
class Module;
class Program;
class Return;
class Arg;
class VarExpr;
enum class Mod;

// ast_compiler.hpp:
class AstCompiler;

// common.hpp:
struct NotImplemented;
struct NotSupported;
struct Unreachable;

// compiler.hpp:
class Compiler;

// console_io.hpp:
class ConsoleError;
class ConsoleOut;

// empty_post_proc.hpp:
class EmptyPostproc;

// empty_pre_proc.hpp:
class EmptyPreproc;


// error.hpp:
struct Error;

// io.hpp:
class ErrorHandler;
class Output;
struct Error;
struct Io;

// parser_interface.hpp:
struct ParserInterface;

// parser_options.hpp:
struct ParserOptions;

// pipeline.hpp:
class Compiler;
class Pipeline;
class Postprocessor;
class Preprocessor;
class TypeStore;

// post_processor.hpp:
class Postprocessor;

// pre_processor.hpp:
class Preprocessor;

// type.hpp:
class Type;
enum class BuiltInType;
struct UnknownType;
struct UserType;

// type_store.hpp:
class TypeStore;

// wasm_compiler.hpp:
class WasmCompiler;
enum class SymbolKind;
struct Symbol;

// wasm_formatter.hpp:
class WasmFormatter;

#endif  // FWD_HPP
