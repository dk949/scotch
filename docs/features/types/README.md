# Types

## Builtin types

``` scot
// Wasm types
const builtin32BitInt: i32 = 0i;
const builtin64BitIntWithSuffix: i64 = 0l;
const builtin64BitIntNoSuffix: i64 = 0;

const builtin32BitFloat: f32 = 0.0f;
const builtin64BitFloatWithSuffix: f64 = 0.0d;
const builtin64BitFloatNoSuffix: f64 = 0.0;

const boolean: bool = false;
def foo(): void{}

const tuple: [i64, i64] = [123, 456];
```

## User types

### Aliases

``` scot
alias MyTupleT = [i32, i32];
let tuple: MyTupleT = [123, 456];
let otherTuple:  [i32, i32] = tuple;
```

### Strong typedef

``` scot
// Probably provided in library form
```

### Structs

``` scot
struct S {
    member0: i32; // Private by default
    pub member1: f64; // Explicitly public
    member2: SomeType;  // User defined type
}

const s: S = ... // Constructor synatx not settled yet
```

## References

WIP
