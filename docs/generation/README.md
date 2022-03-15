# Breakdown of WASM generation

## Test\_1

<table align="center">
<tr>
<th>Scotch</th>
<th>WebAssembly text</th>
</tr>
<tr>
<td>
    
```scotch
module Main;

def main(): i32 {


    return 10i;
}
```
    
</td>
<td>
    
```wasm
;; Note: Actual generation has no whitespace. It was added for clarity.
(module $Main
  (func $main
    (export "main")
    (result i32)
    (local i32)
    (block $main
      (local.set 0 (i32.const 10))
      br $main
    )
    (local.get 0)
  )
)
```
    
</td>
</tr>
</table>

* A WASM module represents a single compilation unit. So every scotch file
  begins by declaring the module.
* The compiled module has been given a name. This is permitted by the WAT
  syntax, but I cannot figure out what it is used for.
* Currently every function is exported. The exported name is the same as the
  name in the source code.
* The declared return type of the function corresponds directly to the generated
  `(result type)` and the declared local variable into which the return value
  will be written.
* Function body is placed inside of a `(block)` with the same name as the
  function. This allows the use of `br` for early return.
* This is also why the return variable exists, since `(block)` does not allow
  fall thought. The return value is written to the variable, block is broken and
  return variable is placed on the stack to be returned from the function.

## Test\_2

<table align="center">
<tr>
<th>Scotch</th>
<th>WebAssembly text</th>
</tr>
<tr>
<td>
    
```scotch
module Main;

def main(): i32 {
    const ret: i32 = 10i;

    const someI32: i32       = 10i;
    const someI64: i64       = 10l;
    const anotherI64: i64    = 10;

    const someF32: f32       = 10f;
    const someF64: f64       = 10d;
    const anotherF32: f32    = 10.3f;
    const anotherF64: f64    = 10.3d;
    const yetAnotherF64: f64 = 10.3;

    return ret;
}
```
    
</td>
<td>
    
```wasm
(module $Main
  (func $main
    (export "main")
    (result i32)
    (local i32)
    (local $ret i32)
    (local $someI32 i32)
    (local $someI64 i64)
    (local $anotherI64 i64)
    (local $someF32 f32)
    (local $someF64 f64)
    (local $anotherF32 f32)
    (local $anotherF64 f64)
    (local $yetAnotherF64 f64)
    (block $main
      (local.set $ret (i32.const 10))
      (local.set $someI32 (i32.const 10))
      (local.set $someI64 (i64.const 10))
      (local.set $anotherI64 (i64.const 10))
      (local.set $someF32 (f32.const 10))
      (local.set $someF64 (f64.const 10))
      (local.set $anotherF32 (f32.const 10.3))
      (local.set $anotherF64 (f64.const 10.3))
      (local.set $yetAnotherF64 (f64.const 10.3))
      (local.set 0 (local.get $ret))
      br $main
    )
    (local.get 0)
  )
)
```
    
</td>
</tr>
</table>

* This example declares many local variables in the function.
* All locals in WASM have to be declared at the top of the function.
* They are assigned value in the same order as they are declared in the source
  code.
* All variables have a name corresponding to the source code.
* Return variable is still present, but does not have a name, so it cannot be
  referred to in the source.
