#include "pipeline.hpp"

#include "ast.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include "macros.hpp"
#include "post_processor.hpp"
#include "pre_processor.hpp"
#include "type_store.hpp"


template<typename I>
auto runArray(auto beginIter, auto endIter, const I &input, auto &&fn) {
    return std::accumulate(beginIter, endIter, ErrorOr<I>(std::move(input)), fn);
}

ErrorOr<std::string> Pipeline::runCompilation(const Program &program, const TypeStore &ts) {
    auto preprocessed = TRY(runArray(m_preprocessor.begin(),
        m_preprocessor.end(),
        program,
        [](ErrorOr<Program> &&a, const std::unique_ptr<Preprocessor> &b) -> ErrorOr<Program> {
            return b->preprocess(TRY(a));
        }));
    m_compiler->setProgram(preprocessed);
    m_compiler->setTypeStore(ts);

    TRY_VOID(m_compiler->typeCheck());

    auto compiled = TRY(m_compiler->compile());

    return runArray(m_postprocessor.begin(),
        m_postprocessor.end(),
        compiled,
        [](ErrorOr<std::string> &&a, const std::unique_ptr<Postprocessor> &b) -> ErrorOr<std::string> {
            return b->postprocess(TRY(a));
        });
}

void Pipeline::outputResult(const ErrorOr<std::string> &in) {
    if (in) {
        m_io.output(in.value());
    } else {
        m_io.error(in.error());
    }
}

void Pipeline::run(const Program &prog, TypeStore ts) {
    outputResult(runCompilation(prog, ts));
}
Pipeline::Pipeline(PtrVec<Preprocessor> &&preprocessor, Ptr<Compiler> &&compiler, PtrVec<Postprocessor> &&postprocessor, Io &&io)
        : m_preprocessor(std::move(preprocessor))
        , m_compiler(std::move(compiler))
        , m_postprocessor(std::move(postprocessor))
        , m_io(std::move(io)) { }
