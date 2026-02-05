#include <iostream>
#include <exception>
#include "AnyType.h"


int main() {
	//std::cout << sizeof(AnyType);

	AnyType a = AnyType(2.1);
	AnyType b = AnyType('A');
	a.Swap(b);

	a.print();
	b.print();

	try {
		a.ToInt();
	}
	catch (std::exception& e) {
		std::cout << e.what();
		std::cout << "Bad";
	}
}