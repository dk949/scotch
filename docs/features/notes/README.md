# Notes

## Constructors

Design of constructors will depend on presence of overloading. If overloading is
allowed, constructors will take the following form:

``` scot
struct S {
  Self() {
  }
}
```

A special member function is introduced, because calling the constructor with
dot syntax is undesirable. Additionally it will still require some
initialisation syntax to create the object in the constructor (like it is done
in rust) Example:

``` scot
struct Point {
  x: i32;
  y: i32;
  // Alternative syntax
  new(x: i32, y: i32): Self {
     const self: Self = ... // Some syntax to initialise self from x and y
     return self;
  }
}
```

## Imports

Depending on what level of support first class namespaces get, there will be
either a library function or a builtin function to import a module inline and
only make it available there. Example:

``` scot
// There exisst some module M providing function fn
module This;

// defines This::fn
def fn(): void {}

fn(); // calls This::fn

// Syntax not final
import(M)::fn(); // calls M::fn. M::fn is not available anywhere else.
```

## References

WIP
