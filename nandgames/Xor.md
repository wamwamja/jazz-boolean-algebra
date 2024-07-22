## Implement an XOR gate using NAND gates

### Derive the expression

The following is the truth table for an XOR gate:

| A | B | XOR |
|---|---|-----|
| 0 | 0 | 0   |
| 0 | 1 | 1   |
| 1 | 0 | 1   |
| 1 | 1 | 0   |

An XOR gate's output is true when the two inputs are different. From the truth table,
it's not difficult to verify that

$$ XOR(A, B) = \overline{AB} \cdot (A+B).$$

Reformulating the expression in terms of NAND gates, we have

$$
\overline{AB} \cdot (A+B) \\
= \overline{AB} A + \overline{AB}B \\
= \overline{\overline{\overline{AB} A + \overline{AB}B}} \\
= \overline{\overline{\overline{AB} A} \cdot \overline{\overline{AB} B}} \\
= NAND(NAND(NAND(A,B), A), NAND(NAND(A,B), B))
$$

### Verify the expression

We can verify the expression by writing a simple C++ program using the `Expr` class.

```cpp
#include "jazz/boolean-algebra.h"
int main() {
    using namespace jazz;
    Expr A("A");
    Expr B("B");
#define NAND(a, b) (!((a) & (b)))
    Expr XOR = NAND(NAND(NAND(A,B), A), NAND(NAND(A,B), B));
    XOR.printTruthTable();
    return 0;
}
```

Compiling and running the above code should print the truth table of the XOR gate.