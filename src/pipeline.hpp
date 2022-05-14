#ifndef PIPELINE_HPP
#define PIPELINE_HPP
#include "ast_fwd.hpp"
#include "compiler.hpp"
#include "error_handler.hpp"
#include "output.hpp"
#include "post_processor.hpp"
#include "pre_processor.hpp"

#include <memory>
#include <numeric>
#include <vector>

class Pipeline {

private:
    template<typename T>
    using Ptr = std::unique_ptr<T>;

    template<typename T>
    using PtrVec = std::vector<std::unique_ptr<T>>;

    PtrVec<Preprocessor> m_preprocessor;
    Ptr<Compiler> m_compiler;
    PtrVec<Postprocessor> m_postprocessor;
    Ptr<Output> m_output;
    Ptr<ErrorHandler> m_error;

    ErrorOr<std::string> runCompilation(Program &&);
    void outputResult(ErrorOr<std::string> &&);

public:
    Pipeline(PtrVec<Preprocessor> &&, Ptr<Compiler> &&, PtrVec<Postprocessor> &&, Ptr<Output> &&, Ptr<ErrorHandler> &&);
    void run(Program &&);
};

#endif  // PIPELINE_HPP
