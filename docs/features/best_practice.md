# Best practice

These rules are not enforced by the compiler (and there is no linter for the
language yet). It's meant to be something like python's pep8. Feel free to
disregard this.

## Modules

### Importing

* Prefer braced import over wildcard import.
* Prefer namespace import over braced import.
* Import deepest nested namespace.
* Wildcard import can be used when importing a module which only exports other
  modules.

``` scot
*- for a module lib containing nested module mod which exports foo and bar

import lib::mod::*;          -- Undesirable.
import lib::mod::{foo, bar}; -- Prefer over the one above.
import lib;                  -- Prefer over the one above.
import lib::mod;             -- Prefer over the one above.

import lib::*; -- Accepatble if lib does not export any non-module identifiers.
```

## Variables

### Declaration

* When declaring a variable, prefer `const` over `let`.
  * Consider if `static` can be used.

## Naming

### Module

* Module names should be in PascalCase.
* File names should match module name in snake\_case.

``` scot
module MyModule; -- file my_module.scot:
```

### Function

* Function name should be in camelCase.

``` scot
def myFunc() {}
```

### Variable

* Variables names should be in camelCase.

``` scot
const myVar = 3;
```

### Types

* Types should be in PascalCase.

WIP example

### Blocks

(If I ever get to implementing them) blocks should be in lowercase.

WIP example
