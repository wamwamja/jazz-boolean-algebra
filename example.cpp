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


    // print truth table
    (p + q).printTruthTable();
    (p * q).printTruthTable();

    return 0;
}
