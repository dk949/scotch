# Glossary

* Bare ID:
  * An identifier matching the regular expression `[a-zA-Z_][a-zA-Z0-9_]*`
  * Not matching any keyword.
* Braced import:
  * Refers to an import of the form
    `NamespacedID::{NamespacedID_0, NamespacedID_1,..., NamespacedID_N}`.
  * `NamespacedID`s 0 through N gen imported into the module namespace.
* Declarator:
  * Keyword preceding the declaration of something
  * `def` for functions, `static`, `const` or `let` for variables.
* Identifier:
  * A name defined by the user (or library)
  * Not one of the keywords.
* Keyword:
  * Builtin word with special syntactic meaning
  * E.g. `module`, `def` or `return`.
* Namespaced ID:
  * an identifier made of Bare IDs separated by `::`.
* Static:
  * Available at compile time.
* Wildcard import:
  * Refers to import of the form `NamespacedID::*`.
  * This imports every identifier exported by `NamespacedID` into the module
    namespace.
