#include <iostream>
#include "AnyType.h"


int main() {
    try {
        std::cout << "Value construction:\n";
        AnyType a = 42;
        std::cout << "int " << a.get<int>() << '\n';

        a = 3.14;
        std::cout << "double " << a.get<double>() << '\n';

        a = true;
        std::cout << "bool " << std::boolalpha << a.get<bool>() << '\n';

        std::cout << "\nCopy constructor:\n";
        AnyType b = a;
        std::cout << "copied bool " << b.get<bool>() << '\n';

        std::cout << "\nMove constructor:\n";
        AnyType c = std::move(a);
        std::cout << "moved bool " << c.get<bool>() << '\n';
        std::cout << "a has value? " << std::boolalpha << a.has_value() << '\n';

        std::cout << "\nAssignment operator:\n";
        b = 100L;
        std::cout << "long " << b.get<long>() << '\n';

        std::cout << "\nSwap\n";
        c = 2.71828f;
        std::cout << "before swap:\n";
        std::cout << "b " << b.get<long>() << '\n';
        std::cout << "c " << c.get<float>() << '\n';

        b.swap(c);

        std::cout << "after swap:\n";
        std::cout << "b (float) " << b.get<float>() << '\n';
        std::cout << "c (long) " << c.get<long>() << '\n';

        std::cout << "\nDestroy:\n";
        b.destroy();
        std::cout << "b has value? " << std::boolalpha << b.has_value() << '\n';

        std::cout << "\nException test:\n";
        try {
            c.get<double>();  
        } catch(const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << '\n';
        }

        std::cout << "\nDifferent fundamental types:\n";
        AnyType d = L'A';
        std::cout << "wchar_t " << d.get<wchar_t>() << '\n';

        d = static_cast<unsigned long long>(9999999);
        std::cout << "unsigned long long " << d.get<unsigned long long>() << '\n';
    }
    catch(const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
    }

    return 0;
}


