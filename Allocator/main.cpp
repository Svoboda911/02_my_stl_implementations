#include <iostream>

// 07.11.26

// Realization of allocator in C++ 11 - C++ 17
template <typename T>
struct Allocator {
    T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T))); // Allocate memory for n objects of type T
    }

    void deallocate(T* ptr, std::size_t n) {
        ::operator delete(ptr);
    }

    template <class... Args>
    void construct(T* ptr, Args&&... args) {
        new (ptr) T(std::forward<Args>(args)...); // Construct an object of type T in the allocated memory using placement new
    }

    void destroy(T* ptr) {
        ptr->~T();
    }
};

int main() {
 
    Allocator<int> alloc;

    int* ptr = alloc.allocate(5);
    alloc.construct(ptr, 42);

    std::cout << *ptr;

    alloc.destroy(ptr);
    alloc.deallocate(ptr, 5);

    return 0;
}



