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


    //    // print truth table
    //    (p + q).printTruthTable();
    //    (p * q).printTruthTable();

    //#define XOR(A, B) (!((A) * (B)) * ((A) + (B)))
    //    Expr a("a"), b("b"), n("n");
    //    (XOR(a, b) * (!a) + !XOR(a, b) * n).printTruthTable({a, b});

    // Canonical forms
    std::cout << "Canonical form 0000" << std::endl;
    Canonical::Constant_0().printTruthTable();
    std::cout << "Canonical form 0001" << std::endl;
    Canonical::And().printTruthTable();
    std::cout << "Canonical form 0010" << std::endl;
    Canonical::X_And_Not_Y().printTruthTable();
    std::cout << "Canonical form 0011" << std::endl;
    Canonical::LeftProjection().printTruthTable();
    std::cout << "Canonical form 0100" << std::endl;
    Canonical::Not_X_And_Y().printTruthTable();
    std::cout << "Canonical form 0101" << std::endl;
    Canonical::RightProjection().printTruthTable();
    std::cout << "Canonical form 0110" << std::endl;
    Canonical::Xor().printTruthTable();
    std::cout << "Canonical form 0111" << std::endl;
    Canonical::Or().printTruthTable();
    std::cout << "Canonical form 1000" << std::endl;
    Canonical::Nor().printTruthTable();
    std::cout << "Canonical form 1001" << std::endl;
    Canonical::Equivalence().printTruthTable();
    std::cout << "Canonical form 1010" << std::endl;
    Canonical::Not_Y().printTruthTable();
    std::cout << "Canonical form 1011" << std::endl;
    Canonical::X_Or_Not_Y().printTruthTable();
    std::cout << "Canonical form 1100" << std::endl;
    Canonical::Not_X().printTruthTable();
    std::cout << "Canonical form 1101" << std::endl;
    Canonical::Not_X_Or_Y().printTruthTable();
    std::cout << "Canonical form 1110" << std::endl;
    Canonical::Nand().printTruthTable();
    std::cout << "Canonical form 1111" << std::endl;
    Canonical::Constant_1().printTruthTable();

    return 0;
}
