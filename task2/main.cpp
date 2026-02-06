#include <iostream>
#include "AnyType.hpp"

int main() {
    AnyType myValue = 42; 
    std::cout << "int value: " << myValue.getValue<int>() << std::endl;

    myValue = 3.14159;
    std::cout << "double value: " << myValue.getValue<double>() << std::endl;

    try {
        myValue.getValue<int>();
    } catch (const std::exception& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }

    myValue = true;
    std::cout << "bool value: " << std::boolalpha << myValue.getValue<bool>() << std::endl;

    myValue.reset();
    std::cout << "Type after reset: " << static_cast<int>(myValue.getType()) << std::endl;

    return 0;
}
