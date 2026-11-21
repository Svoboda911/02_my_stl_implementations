#include <iostream>
#include "MyVector.h"
#include <vector>

// struct for emplace_back
/*
struct Human {
    int health;
    std::string name;
    Human(std::string name, int health)
    : health(health), name(std::move(name)) {}
};
*/

int main() {
    MyVector<int> a = {1, 2, 3, 4, 5};

    MyVector<int> b(a);

    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i] << std::endl;
    }

    // b = a; // copy assignment

    for (int i = 0; i < b.size(); i++) {
        std::cout << b[i] << std::endl;
    }

    // emplace_back
    /*
    MyVector<Human> a;

    a.emplace_back("Oleg", 100);

    for (size_t i = 0; i < a.size(); i++) {
        std::cout << a[i].name << " "
                << a[i].health << std::endl;
    }
    */

    return 0;
}