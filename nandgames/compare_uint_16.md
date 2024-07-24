Compare two 16-bit unsigned integers
====================================

In this section, we will implement a 16-bit unsigned integer `less` comparator, which can be used in the level `Add signed magnitude` on the website [The Nand Game](https://nandgame.com).

The `less` comparator accepts two 16-bit unsigned integers `A` and `B` and outputs`1` if `A < B`, `0` otherwise.

### Design idea

At this level, we have a series of components for signed integers available:
- `inv16`: invert 16 bits
- `inc16`: increment 16 bits
- `add16`: add two 16-bit signed integers
- `sub16`: subtract two 16-bit signed integers
- `is_neg16`: check if a 16-bit signed integer is negative

We know that signed integers can be compared by subtracting them and checking the sign bit. However, we don't have a direct way to subtract two 16-bit unsigned integers.  Signed and unsigned integers are only different in the way they are interpreted. 
The highest bit of a signed integer is the sign bit, which is `1` for negative numbers and `0` for positive numbers. It's obvious that for a 16-bit number leads with `0`, interpreting it as a signed or unsigned integer gives the same value. Thus, if both the bits of `A` and `B` lead with `0`, we can directly subtract them by `sub16` and check the sign bit to determine the result. Upon this observation, we can design the `less` comparator as follows:

* If both the highest bits of `A` and `B` are `0`, then `less(A,B) = sub16(A, B) < 0`.
* If the highest bit of `A` is `1` and `B` is `0`, then `A>B` and `less(A,B) = 0`.
* If the highest bit of `A` is `0` and `B` is `1`, then `A<B` and `less(A,B) = 1`.
* If both the highest bits of `A` and `B` are `1`, the `sub16` method still works because `(0x7fff & A)  -(0x7fff & B) = A - B`.

### Implementation
The design above can be summarized by the truth table below.

| a=is_neg(A) | b=is_neg(B) | c=lt(A,B)          |
|-------------|-------------|--------------------|
| 0           | 0           | n=is_neg(sub(A,B)) |
| 0           | 1           | 1                  |
| 1           | 0           | 0                  |
| 1           | 1           | n=is_neg(sub(A,B)) |

One of its equivalent propositions can be

$$ XOR(a,b)\bar{a}+\overline{XOR(a,b)}n. $$

The proposition can be verified using the library

```cpp
#define XOR(A, B) (!((A) * (B)) * ((A) + (B)))
Expr a("a"), b("b"), n("n");
(XOR(a, b) * (!a) + !XOR(a, b) * n).printTruthTable({a, b});
```

