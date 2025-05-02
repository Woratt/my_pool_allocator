#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <iostream>
#include <vector>

template<typename T, size_t SIZE>
class Pool;

namespace MyLib{

template<typename T>
class Allocator
{
    Pool<T, 1000000>* pool;
    static size_t point;

    public:
    using value_type = T; 
    Allocator();
    Allocator(Allocator<T>&);
    ~Allocator();
    T* allocate(size_t);
    void deallocate(T*, size_t) noexcept;
    template<typename... Args>
    void construct(T*, Args&&...) noexcept;
    void destroy(T*) noexcept;
};

template<typename T>
 size_t Allocator<T>::point = 1;

template<typename T>
Allocator<T>::Allocator(){
    pool = new Pool<value_type, 1000000>;
}

template<typename T>
Allocator<T>::Allocator(Allocator<T>& copy){
    ++point;
    pool = copy.pool;
}


template<typename T>
Allocator<T>::~Allocator(){
    if(point == 1){
    operator delete(pool);
    }else --point;
}

template<typename T>
T* Allocator<T>::allocate(size_t size) {
    T* ptr = pool->alloc();
    if (!ptr) {
        throw std::bad_alloc();
    }

    return ptr;
}

template<typename T>
template<typename... Args>
void Allocator<T>::construct(T* p, Args&&... args) noexcept{
    new (p) T(std::forward<Args>(args)...);
}

template<typename T>
void Allocator<T>::deallocate(T* p, size_t size) noexcept{
    pool->free(p);
}

template<typename T>
void Allocator<T>::destroy(T* p) noexcept{
    p->~T();
}

}

#endif