#include "AnyType.h"
#include <iostream>

int main()
{
    AnyType anyType = 1;
    anyType = true;
    anyType = 1.7;

    try
    {
        int storedValue = anyType.toInt();
    }
    catch (const std::bad_cast&)
    {
        std::cout << "Bad cast double -> int" << std::endl;
    }

    double storedValue = anyType.toDouble();

    std::cout << storedValue << "\n";
}