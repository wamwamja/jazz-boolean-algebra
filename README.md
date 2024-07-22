![CMake Multi-Platform Build](https://github.com/wamwamja/jazz-boolean-algebra/actions/workflows/cmake-multi-platform.yml/badge.svg)

Introduction
=========
It is a C++ library for symbolic boolean algebra calculations.
The design takes the idea of [GiNac](https://www.ginac.de),
which is a C++ library for symbolic mathematical calculations.

I wrote this library to help me solve problems on the website [The Nand Game](https://nandgame.com).
This website explains building a computer from scratch using only NAND gates.

The library obeys the GNU General Public License v3.0.

How to build
=========
It follows the standard CMake build process. Optionally, `gtest` is required to build the tests.

Usage
=====
Currently, the library only supports the following objects and operations:

- Symbol, wrapped in the `Expr` class
- NOT, represented by operator `!`
- AND, represented by operator `&` or `*`
- OR, represented by operator `|` or `+`
- Relation, represented by the relevant operators `==`, `!=`, `>`, `>=`, `<`, `<=`
- Substitution, represented by the `Expr::subs()` method

A user only needs to care about the `Expr` class among all the internal structures because `Expr` wraps all of them. One
can build complex boolean expressions with just `Expr` and the overloaded operators. The following example code
demonstrates the usage of the library:

```cpp
#include "jazz/boolean-algebra.h"
#include <iomanip>

#define print(v, x)                                        \
    Expr v = x;                                            \
    std::cout << std::setw(6) << std::left << #v << " : "; \
    std::cout << std::setw(40) << std::left << #x << " => " << (x) << std::endl

using namespace jazz;
int main() {

    // Define some symbols
    Expr p("p");
    Expr q("q");
    Expr r("r");
    Expr s("s");
    Expr t("t");
    Expr u("u");

    // build some expressions
    print(expr0, p + true);
    print(expr1, p * false);
    print(expr2, p + p);
    print(expr3, p + !p);
    print(expr4, p * p);
    print(expr5, p * !p);
    print(expr6, p + q + r);
    print(expr7, expr6.subs(p == false));
    print(expr8, expr7.subs(q == false));
    print(expr9, expr8.subs(r == false));
    print(expr10, (p + q + r) * (s + t + u));
    print(expr11, expr10.subs(p == false));
    print(expr12, expr10.subs(u == true));

    return 0;
}
```

The output of the above code may look like this:

```console
expr0  : p + true                                 => 1
expr1  : p * false                                => 0
expr2  : p + p                                    => p
expr3  : p + !p                                   => 1
expr4  : p * p                                    => p
expr5  : p * !p                                   => 0
expr6  : p + q + r                                => q | r | p
expr7  : expr6.subs(p == false)                   => q | r
expr8  : expr7.subs(q == false)                   => r
expr9  : expr8.subs(r == false)                   => 0
expr10 : (p + q + r) * (s + t + u)                => (q | r | p) & (t | u | s)
expr11 : expr10.subs(p == false)                  => (t | u | s) & (q | r)
expr12 : expr10.subs(u == true)                   => q | r | p
```

Issues
======
Be careful when using the library because:

- The library is under development, so many features are missing.
- Unit tests only cover a small portion of the code.

Issues and pull requests are welcome.

Have fun!
=========

* [Xor](nandgames/Xor.md) - Implement an XOR gate using NAND gates.
