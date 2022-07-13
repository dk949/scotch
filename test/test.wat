(module
  $Main
  (func
    $main
    (export "main")
    
    (result i32)
    (local $b i32)
    (i32.const 1)
    (local.set $b)
    (local.get $b)
    return
    )
  )

