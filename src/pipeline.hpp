#ifndef PIPELINE_HPP
#define PIPELINE_HPP
#include "ast_fwd.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include "io.hpp"
#include "post_processor.hpp"
#include "pre_processor.hpp"

#include <memory>
#include <numeric>
#include <vector>

class TypeStore;
class Preprocessor;
class Postprocessor;
class Compiler;

class Pipeline {

private:
    template<typename T>
    using Ptr = std::unique_ptr<T>;

    template<typename T>
    using PtrVec = std::vector<std::unique_ptr<T>>;

    PtrVec<Preprocessor> m_preprocessor;
    Ptr<Compiler> m_compiler;
    PtrVec<Postprocessor> m_postprocessor;
    Io m_io;

    ErrorOr<std::string> runCompilation(const Program &, const TypeStore &);
    void outputResult(const ErrorOr<std::string> &);

public:
    Pipeline(PtrVec<Preprocessor> &&, Ptr<Compiler> &&, PtrVec<Postprocessor> &&, Io &&);
    void run(const Program &, TypeStore );
};

#endif  // PIPELINE_HPP
