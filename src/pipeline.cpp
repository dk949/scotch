#include "pipeline.hpp"

#include "ast.hpp"
#include "error.hpp"
#include "macros.hpp"


template<typename I>
auto runArray(auto beginIter, auto endIter, I &&input, auto &&fn) {
    return std::accumulate(beginIter, endIter, ErrorOr<I>(std::move(input)), fn);
}

ErrorOr<std::string> Pipeline::runCompilation(Program &&program) {
    auto preprocessed = TRY(runArray(m_preprocessor.begin(),
        m_preprocessor.end(),
        std::move(program),
        [](ErrorOr<Program> &&a, const std::unique_ptr<Preprocessor> &b) -> ErrorOr<Program> {
            return b->preprocess(TRY(a));
        }));

    TRY_VOID(m_compiler->typeCheck(preprocessed));

    auto compiled = TRY(m_compiler->compile(std::move(preprocessed)));

    return TRY(runArray(m_postprocessor.begin(),
        m_postprocessor.end(),
        std::move(compiled),
        [](ErrorOr<std::string> &&a, const std::unique_ptr<Postprocessor> &b) -> ErrorOr<std::string> {
            return b->postprocess(TRY(a));
        }));
}

void Pipeline::outputResult(ErrorOr<std::string> &&in) {
    if (in) {
        m_output->output(std::move(in.value()));
    } else {
        m_error->error(std::move(in.error()));
    }
}

void Pipeline::run(Program &&prog) {
    outputResult(runCompilation(std::move(prog)));
}
Pipeline::Pipeline(PtrVec<Preprocessor> &&preprocessor,
    Ptr<Compiler> &&compiler,
    PtrVec<Postprocessor> &&postprocessor,
    Ptr<Output> &&output,
    Ptr<ErrorHandler> &&error)
        : m_preprocessor(std::move(preprocessor))
        , m_compiler(std::move(compiler))
        , m_postprocessor(std::move(postprocessor))
        , m_output(std::move(output))
        , m_error(std::move(error)) { }
