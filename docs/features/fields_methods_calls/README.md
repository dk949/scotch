# Fields, Methods and function calls

## Declaring struct methods

Have not decided on const ref/mutable ref syntax for arguments, see notes at the
end.

``` scot
struct S {
  mem0: i32;
  pub mem1: i64;
  pub staticMethod(): void;
  // alias Self = S; // this is added by the compiler
  pub getMem0(self: Self): i32; // pass self by const ref
  pub setMem0(self: Self, mem: i32): void; // pass self by mutable ref
}
def staticMethod(): void{}

def getMem0(self: Self): i32{
  ... // see below for member access syntax
}

def setMem0(self: Self, mem: i32): void{
  ... // see below for member access syntax
}
```

## Bracket call syntax

``` scot
def noArgFn(): void {}
def oneArgFn(i: i32): void {}
def manyArgFn(i: i32, f: f32): void {}

struct S {
    pub member: i32;
    pub staticMethod(): void;
    pub noArgMethod(self: Self): void;
    pub oneArgMethod(self: Self, i: i32): void;
}

noArgFn(); // no arg call
oneArgFn(42); // 1 arg call
manyArgFn(3, 2.5); // many arg call

const s: S = ... // Construction

const i = S::member(s); // accessing member

S::staticMethod(); // static method call
S::noArgMethod(s); // instance method no argument call
S::oneArgMethod(s, 42); // instance method one argument call
```

## Dot call syntax

``` scot
def noArgFn(): void {}
def oneArgFn(i: i32): void {}
def manyArgFn(i: i32, f: f32): void {}

struct S {
    pub member: i32;
    pub staticMethod(): void;
    pub noArgMethod(self: Self): void;
    pub oneArgMethod(self: Self, i: i32): void;
}

noArgFn(); // no arg call. no dot syntax
42.oneArgFn(); // 1 arg call
42.oneArgFn; // 1 arg call. brackets ommissible
3.manyArgFn(2.5); // many arg call
const s: S = ... // Construction

const i = s.member(); // accessing member
const i = s.member; // accessing member. brackets ommissible

S::staticMethod(); // static method call. no dot syntax
s.S::noArgMethod(); // instance method no arg.
s.noArgMethod(); // instance method no arg. Namespace ommissible.
s.noArgMethod; // instance method no arg. Namespace ommissible and brackets ommissible
s.S::oneArgMethod(42); // instance method one arg.
s.oneArgMethod(42); // instance method one arg. Namespace ommissible.
```
