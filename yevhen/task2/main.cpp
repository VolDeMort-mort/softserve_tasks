#include <iostream>
#include <thread>
#include "src/AnyType.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    
    AnyType a = 10;
    std::thread t1 ([&a]() {
        for (int i = 0; i < 100000; ++i) {
            a = i;
        }
    });
    std::thread t2 ([&a]() {
        for (int i = 0; i < 100000; ++i) {
            auto t = a.ToInt();
            a = t + 1;
        }
    });

    t1.join();
    t2.join();
    std::cout << "Final value: " << a.ToInt() << std::endl;
    
    return 0;
}