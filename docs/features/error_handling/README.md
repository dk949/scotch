# Error handling

Error handling is done through static exceptions. Every function which may
`fail` has to declare so.

## Signature

In this example `RetType` is the return type of the function and `ErrorType` is
the type of error which the function can fail with.

``` scot
def foo(): RetType err ErrorType
```

Return type can still be deduced. Error type cannot be deduced.

``` scot
def foo() err ErrorType
```

## Throwing an exception

Throwing an exception looks just like returning a value, but `fail` is used
instead of `return`.

``` scot
def foo() err Error {
    const shouldFail = true;
    if (shouldFail)
        fail makeError();
}
```

## Catching exceptions

If a function `a` throwing type `T` is called from a function `b` throwing type
`T`, result of `a` can be used as if `a` could not fail. If `a` does fail, the
error is rethrown.

``` scot
def foo() err Error {
  fail makeError();
  return 42;
}
def bar() err Error {
  const var = foo(); -- bar fails here
  return var;
}
```

If a function `a` throwing type `T` is called from a function `b` throwing type
`U` (including if `U` is `void`), `a` cannot be called outside of a `catch`.

`catch` expression consists of calling the possibly failing function in the
parenthesise and returning the following expression in case of failure.

``` scot
def foo() err Error {
  fail makeError();
  return 42;
}

def bar() {
  const var = catch(foo()) 42;
  return var;
}
```

If the variable declaration expression is used as part of the catch argument,
the value of the variable will be the error in case of a failure. This allows
errors to be converted to other types if needed.

Note: this specific use case may get a library function, or even a dedicated
operator.

``` scot
def foo() err Error {
  fail makeError();
  return 42;
}

def bar() OtherError{
  const var = catch(const e = foo()) fail fromError(e);
  return var;
}
```

An `else` expression can follow the `catch` expression and will be executed if
no error occurred.

WIP on using the non erroneous result of a function in the `else`.

``` scot
def foo() err Error {
  return 42;
}

def bar() OtherError{
  const var =
    catch(const e = foo())
      fail fromError(e)
    else
      42;

  return var;
}
```

Possible use cases include calling multiple functions in the `catch` argument,
as long as they have the same error type.

```scot
def f0() err Error {
  fail makeError();
}

def f1() err Error {
  fail makeError();
}

def f2() err Error {
  fail makeError();
}

def bar() OtherError{
  catch(const e = {
    f0();
    f1();
    f2();
  }) fail fromError(e);
  return var;
}
```

## Exception types

Any type can be used as an exception type.
