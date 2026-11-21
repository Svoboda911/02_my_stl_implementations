#pragma once
#include <iostream>
#include <cstddef>

template <typename T, typename Alloc = std::allocator<T>>
class MyVector {
private:
    Alloc alloc;
    T* data;
    std::size_t current_size;
    std::size_t capacity;
public:
    MyVector();
    MyVector(std::size_t input_size); //overloading of constructor
    MyVector(const std::initializer_list<T>& list); //overloading of constructor

    MyVector(const MyVector& other); // II. copy constructor
    MyVector& operator=(const MyVector& other); // III. copy assignment operator

    std::size_t size() const;
    T& operator[](std::size_t index);
    T& at(std::size_t index);

    void resize(std::size_t new_size);
    void reserve(std::size_t new_capacity);

    void push_back(const T& value);
    void pop_back();

    void shrink_to_fit();

    template <typename... Args>
    void emplace_back(Args&&... args);

    ~MyVector(); // I. Destructor
};

#include "MyVector.tpp"

/*
Rule Of Five:
I. Destructor
II. Copy constructor
III. Copy assignment
IV. Move constructor
V. Move assignment
*/