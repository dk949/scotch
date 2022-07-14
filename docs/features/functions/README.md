# Functions

For function call syntax see [Fields, Methods and function
calls](../fields_methods_calls/README.md).

``` scot
def main(): i32{
    return 0i;
}
```

## Declaration

### Signature

``` scot
def func(): f32
```

See [Error handling](../error_handling/README.md).

``` scot
def func(): i32 err Error
```

### Body

``` scot
def foo(): i64 {
    return 42;
}

def bar(): i64 42; -- braces are optional, as is the semicolon at the end
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
def foo(){ -- The return type of the function is derived to be `i64`.
  return 42;
}
```

## Order of declarations

WIP in the implementation

``` scot
def bar(): i32 {
    return foo(); -- Can be called before it is declared
}
def foo(): i32 {
    return 42i;
}
```
