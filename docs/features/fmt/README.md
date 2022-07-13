# Formatting

Language level string interpolation. See
[this](https://forum.dlang.org/thread/jyzytxcorezmnyfbhbbw@forum.dlang.org)
thread on the dlang forum as to why it's not a library feature.

## Basics

Variables and expressions can be interpolated into strings (BAI they are
converted to strings and concatenated in the middle of a string).

An interpolated string begins and ends with backticks (\`). Anything appearing
between curly braces ({}) has has to be a valid scotch expression. The
expression gets evaluated and it's result has to be formattable.

``` scot
const v = 42;
const varInterp = `the answer is {v}`;
// "the answer is 42"

const exprInterp = `the answer is {40 + 2}`;
// "the answer is 42"

const literalBrace = `this is a brace: {{`;
// "this is a brace: {"
```

## Format specs

There are various format specifiers. They can be supplied after an optional
semicolon. Multiple format specifiers are separated by semicolons.

### `size`

* Sets the amount of horizontal space used to display an expression.
* Unit: i32
* Default: size of the expression as a string.
* Note: will likely be used in conjunction with other specifiers to correctly
  position text.

### `overflow`

* What to do if expression does not fit the requested size.
* Unit: enum {Grow, TruncateL,TruncateR, Error}
  * Grow = size acts as minimum size, but can be expanded.
  * TruncaseR = align the expression to the left side of the allowed space and
    print \[size\] characters. Discard the rest.
  * TruncaseL = align the expression to the right side of the allowed space and
    print \[size\] characters to the left. Discard the rest.
  * Error: Throw an error if unable to fit expression in size.
* Default: Grow
* Note: No effect if size == expression size.

### `align`

* when expression is smaller than size, which side should it be aligned to.
* Unit: enum {Left, CenterLeft,CenterRight, Right}
  * CenterLeft and CentwrRight differ if `size % 2 != exprSize % s`
  * CenterLeft = one character to the left of center.
  * CenterRight = one character to the right of center.
* Default: Left
* Note: No effect if size == expression size.

### `fill`

* Character used to fill the empty space when expression is smaller than size.
* Unit: char
* Default: ' '
* Note: No effect if size == expression size.

``` scot
const i = 3;

const rightPadWithSpace= `{i; size = 5}`;
// "3    "
const leftPadWith0= `{i;size=5;align=Left;fill='0'}`;
// "00003"
```

## Decimal point precision

WIP

## Debug format spec

WIP

## Notes

Should the user be able to set format specs dynamically? Probably yes.

``` scot
const i = 3;
let size = 5;
const dynaicPad = `number = {i; size = size}`;
```

If format specs can be set dynamically, can enum types used for alignment and
overflow specification be available outside of the interpolated string context?

If yes, are they provided by the standard library? What happens when format spec
enum names clash with user defined values (because they are not namespaced).
