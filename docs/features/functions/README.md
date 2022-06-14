# Functions

For function call syntax see [Fields, Methods and function
calls](../fields_methods_calls/README.md).

``` scot
def main(): i32{
    return 0i;
}
```

## Declaration

## Signature

``` scot
def func(): f32
```

## Body

``` scot
def foo(): i64 {
    return 42;
}
// Equivalent to the above
def bar(): i64 => 42;
```

## Parameters

### Declaration

``` scot
def foo(i: i32, l: i64, f: f32, d: f64): i32 {
    return i;
}
```

### Passing

WIP. Builtin types are passed by value (copied).

User types... will decide later, probably by value.

## Return type deduction

``` scot
// The return type of the function is derived to be `i64`.
def foo(){
  return 42;
}
```

## Order of declarations

``` scot
def bar(): i32 {
    return foo(); // Can be called before it is declared
}
def foo(): i32 {
    return 42i;
}
```
