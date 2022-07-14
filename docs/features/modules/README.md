# Modules and namespaces

## Declaring module

Module declaration consists of the `module` keyword, followed by a bare ID.

``` scot
module ModuleName; -- defines current file as module named ModuleName
```

## Importing modules


``` scot
import OtherModule;      -- importing a module named OtherModule
import Another::hello;   -- Importing only the export hello from module Another
import More::{foo, bar}; -- Importing foo and bar from More.
import EvenMore::*;      -- Importing everything exported from EvenMore.


const t: OtherModule::Thing = ... -- accessing Thing exporetd from OtherModule

Another::hello(); -- calling hello imported from Another
hello(); -- Namespace ommissible

-- calling foo and bar imported from More, namespace ommissible
More::foo();
bar();

-- calling baz imported from EvenMore, namespace ommissible
EvenMore::baz();
baz();
```


## Defining module exports
WIP

## Multiple modules in a file
WIP
