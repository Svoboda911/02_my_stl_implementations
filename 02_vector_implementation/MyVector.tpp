#include "MyVector.h"

/*
Allocators:
using traits = std::alocator_traits<Allocator>
placement new -> traist::constuct();
reinterpret_cast -> traits::allocate();
ptr->~arr -> traits::destroy();
delete ptr; -> traits::deallocate();
*/

// MyVector class implementation
template <typename T, typename Alloc>
MyVector<T, Alloc>::MyVector() {
    using traits = std::allocator_traits<Alloc>;
    current_size = 0;
    capacity = 0;

    // create an dynamic empty array
    // data = new T[capacity];
    data = nullptr;
}

// Implementation of the overloaded constructor
template <typename T , typename Alloc>
MyVector<T, Alloc>::MyVector(std::size_t input_size) { 
    using traits = std::allocator_traits<Alloc>;
    capacity = std::move(input_size);
    current_size = std::move(input_size);

    // create a dynamic array
    data = traits::allocate(alloc, capacity);

    // Construct the elements in the allocated memory
    std::size_t i = 0;
    try {
        for (; i < current_size; ++i) {
            traits::construct(alloc, data + i);
        }    
    }
    catch(...) { // If an exception occurs during construction, destroy the already constructed elements and deallocate the memory
        while (i > 0) {
            --i;
            traits::destroy(alloc, data + i);
        }

        traits::deallocate(alloc, data, capacity);
        throw;
    }
}

// Implementation of the constructor that takes an initializer list
template <typename T, typename Alloc>
MyVector<T, Alloc>::MyVector(const std::initializer_list<T>& list) {
    using traits = std::allocator_traits<Alloc>;

    if (list.size() == 0) return;

    capacity = list.size();
    current_size = 0;
    data = traits::allocate(alloc, capacity);

    try {
        for (const T& element : list) {
            traits::construct(alloc, data + current_size, element);
            ++current_size;
        }
    }
    catch(...) {
        while (current_size > 0) {
            --current_size;
            traits::destroy(alloc, data + current_size);
        }

        traits::deallocate(alloc, data, capacity);
        throw;
    }
}

// Implementation of the copy constructor
template <typename T, typename Alloc>
MyVector<T, Alloc>::MyVector(const MyVector& other) {
    using traits = std::allocator_traits<Alloc>;

    size_t new_current_size = 0;
    
    size_t new_capacity = other.capacity;
    T* new_data = traits::allocate(alloc, new_capacity);

    try {
        for (size_t i = 0; i < other.current_size; ++i) {
            traits::construct(alloc, new_data + new_current_size, other.data[i]);
            ++new_current_size;
        }
    }
    catch(...) {
        while (new_current_size > 0) {
            --new_current_size;
            traits::destroy(alloc, new_data + new_current_size);
        }

        traits::deallocate(alloc, new_data, new_capacity);

        throw;
    }

    capacity = other.capacity;
    current_size = new_current_size;
    data = new_data;
}

// Implementation of the copy assigmnent
template <typename T, typename Alloc>
MyVector<T, Alloc>& MyVector<T, Alloc>::operator=(const MyVector<T, Alloc>& other) {
    using traits = std::allocator_traits<Alloc>;

    if (this == &other) return *this;

    size_t new_current_size = 0;
    
    size_t new_capacity = other.capacity;
    T* new_data = traits::allocate(alloc, new_capacity);

    try {
        for (size_t i = 0; i < other.current_size; ++i) {
            traits::construct(alloc, new_data + new_current_size, other.data[i]);
            ++new_current_size;
        }
    }
    catch(...) {
        while (new_current_size > 0) {
            --new_current_size;
            traits::destroy(alloc, new_data + new_current_size);
        }

        traits::deallocate(alloc, new_data, new_capacity);

        throw;
    }
    
    for (size_t i = 0; i < current_size; ++i) {
        traits::destroy(alloc, data + i);
    }
    traits::deallocate(alloc, data, capacity);

    capacity = other.capacity;
    current_size = new_current_size;
    data = new_data;
    return *this;
}
/*
propagate_on_container_copy_assignment;
select_on_container_copy_construction;
propagate_on_container_swa
*/

// Implementation of the size() method
template <typename T, typename Alloc>
std::size_t MyVector<T, Alloc>::size() const {
    return current_size;
}

// Implementation of the [] operator
template <typename T, typename Alloc>
T& MyVector<T, Alloc>::operator[](std::size_t index) {
    return data[index];
}

// Implementation of the at() method 
template <typename T, typename Alloc>
T& MyVector<T, Alloc>::at(std::size_t index) {
    if (index >= current_size) throw std::out_of_range("...");
    return data[index];
}

// Implementation of the reserve() method for lvalue
template <typename T, typename Alloc>
void MyVector<T, Alloc>::reserve(std::size_t new_capacity) {
    if (new_capacity > capacity) {
        using traits = std::allocator_traits<Alloc>;
        // T* new_data = reinterpret_cast<T*>(new char[new_capacity * sizeof(T)]);
        T* new_data = traits::allocate(alloc, new_capacity);

        // Move the existing elements to the new array
        std::size_t i = 0;
        try {
            for (; i < current_size; ++i) {
                traits::construct(alloc, new_data + i, data[i]);
            }
        }
        catch(...) {
            while (i > 0) {
                --i;
                traits::destroy(alloc, new_data + i);
            }

            traits::deallocate(alloc, new_data, new_capacity);
            throw;
        }

        // Destroy the old elements and deallocate the old array
        for (std::size_t i = 0; i < current_size; ++i) {
            traits::destroy(alloc, data + i);
        }
        
        traits::deallocate(alloc, data, capacity);
        data = new_data;
        capacity = new_capacity;
    }
}

// Implementation of the resize() method
template <typename T, typename Alloc>
void MyVector<T, Alloc>::resize(std::size_t new_size) {
    if (new_size > capacity) {
        reserve(new_size);
    }

    if (new_size > current_size) {
        using traits = std::allocator_traits<Alloc>;
        for (std::size_t i = current_size; i < new_size; ++i) {
            traits::construct(alloc, data + i);
        }
    }

    else if (new_size < current_size) {
        using traits = std::allocator_traits<Alloc>;
        for (std::size_t i = new_size; i < current_size; ++i) {
            traits::destroy(alloc, data + i);
        }
    }

    current_size = new_size;
}

// Implementation of the push_back() method for lvalue
template <typename T, typename Alloc>
void MyVector<T, Alloc>::push_back(const T& x) {
    using traits = std::allocator_traits<Alloc>; // Use allocator traits to access the allocator's methods

    
    if (current_size == capacity) {
        std::size_t new_capacity = capacity == 0 ? 1 : capacity * 2;

        reserve(new_capacity);
    }
    traits::construct(alloc, data + current_size, x);
    ++current_size;
}

// Implementation of the overloading push_back() method for rvalue
template <typename T, typename Alloc>
void MyVector<T, Alloc>::push_back(T&& x) {
    using traits = std::allocator_traits<Alloc>; // Use allocator traits to access the allocator's methods

    if (current_size == capacity) {
        std::size_t new_capacity = capacity == 0 ? 1 : capacity * 2;

        if (new_capacity > capacity) {
            std::cout << "rvalue push_back()\n";
            using traits = std::allocator_traits<Alloc>;
            // T* new_data = reinterpret_cast<T*>(new char[new_capacity * sizeof(T)]);
            T* new_data = traits::allocate(alloc, new_capacity);

            // Move the existing elements to the new array
            std::size_t i = 0;
            try {
                for (; i < current_size; ++i) {
                    traits::construct(alloc, new_data + i, std::move(data[i]));
                }
            }
            catch(...) {
                while (i > 0) {
                    --i;
                    traits::destroy(alloc, new_data + i);
                }

                traits::deallocate(alloc, new_data, new_capacity);
                throw;
            }

            // Destroy the old elements and deallocate the old array
            for (std::size_t i = 0; i < current_size; ++i) {
                traits::destroy(alloc, data + i);
            }
            
            traits::deallocate(alloc, data, capacity);
            data = new_data;
            capacity = new_capacity;
        }
    }
    traits::construct(alloc, data + current_size, std::move(x));
    ++current_size;
}

// Implementation of the pop_back() method
template <typename T, typename Alloc>
void MyVector<T, Alloc>::pop_back() {
    using traits = std::allocator_traits<Alloc>;

    if (current_size == 0) return;

    --current_size;
    traits::destroy(alloc, data + current_size);
}

// Implementation of the assign() method 
template <typename T, typename Alloc>
void MyVector<T, Alloc>::assign(size_t count, const T& value) {
    using traits = std::allocator_traits<Alloc>;

    if (count > capacity) {
        std::size_t new_capacity = count * 2;

        T value_copy = value;
        T* new_data = traits::allocate(alloc, new_capacity);

        size_t i = 0;
        try {
            for (; i < count; ++i) {
                traits::construct(alloc, new_data + i, value_copy);
            }      
        }
        catch(...) {
            while (i > 0) {
                --i;
                traits::destroy(alloc, new_data + i);
            }

            traits::deallocate(alloc, new_data, new_capacity);

            throw;
        }

        // Destroy the old elements and deallocate the old array
        for (std::size_t i = 0; i < current_size; ++i) {
            traits::destroy(alloc, data + i);
        }

        if (data != nullptr)
            traits::deallocate(alloc, data, capacity);
        
        data = new_data;
        capacity = new_capacity;
        current_size = count;

        return;
    }
    T value_copy = value;

    // Destroy the old elements and deallocate the old array
    for (size_t i = 0; i < current_size; ++i) {
        traits::destroy(alloc, data + i);
    }

    size_t i = 0;
    try {  
        for (; i < count; ++i) {
            traits::construct(alloc, data + i, value_copy);
        }
    }
    catch(...) {
        while (i > 0) {
            --i;
            traits::destroy(alloc, data + i);
        }

        traits::deallocate(alloc, data, capacity);
        throw;
    }

    current_size = count;
}

// Implementation of the emplace_back() method
template <typename T, typename Alloc>
template <typename... Args>
void MyVector<T, Alloc>::emplace_back(Args&&... args) {
    using traits = std::allocator_traits<Alloc>;

    if (current_size == capacity) {
        std::size_t new_capacity = capacity == 0 ? 1 : capacity * 2;

        reserve(new_capacity);
    }
    traits::construct(alloc, data + current_size, std::forward<Args>(args)...);
    ++current_size;
}

// Implementation of the shrink_to_fit() method
template <typename T, typename Alloc>
void MyVector<T, Alloc>::shrink_to_fit() {
    using traits = std::allocator_traits<Alloc>;

    if (current_size == capacity) return;

    if (current_size == 0) {
        if (data != nullptr) traits::deallocate(alloc, data, capacity);

        data = nullptr;
        capacity = 0;
        return;
    }

    T* new_data = traits::allocate(alloc, current_size);

    size_t i = 0;
    try {
        for (; i < current_size; ++i) {
            traits::construct(alloc, new_data + i, std::move(data[i]));
        }
    }
    catch(...) {
        while (i > 0) {
            --i;
            traits::destroy(alloc, new_data + i);
        }

        traits::deallocate(alloc, new_data, current_size);
        throw;
    }

    for (size_t i = 0; i < current_size; ++i) {
        traits::destroy(alloc, data + i);
    }
    
    traits::deallocate(alloc, data, capacity);

    data = new_data;
    capacity = current_size;
}

// Implementation of the destructor
template <typename T, typename Alloc>
MyVector<T, Alloc>::~MyVector() {
    using traits = std::allocator_traits<Alloc>;
    
    for (std::size_t i = 0; i < current_size; ++i) {
        traits::destroy(alloc, data + i);
    }

    traits::deallocate(alloc, data, capacity);
}



/*

class C {
public:
    template <typename U>
    C(const U&) = delete;
};

*/