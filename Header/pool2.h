#ifndef POOL2_H
#define POOL2_H
#include <iostream>

namespace MyLib {
    template<typename T> class Allocator;
}

template<typename T, size_t SIZE>
class Pool
{
    ~Pool(){}
    static const size_t BITS_IN_UINT8 = 8;
    static const size_t NO_BYTES = (SIZE + BITS_IN_UINT8 - 1) /BITS_IN_UINT8;   

    alignas(T) char elements[sizeof(T) * SIZE];

    uint8_t info[NO_BYTES];
    size_t free_element;

    bool testBit(size_t bit_index);
    void setBit(size_t bit_index);
    void clrBit(size_t bit_index);

    public:

    Pool();
    T* alloc();
    void free(T* p);
    int size();
    int pos = 0;
    friend class MyLib::Allocator<T>;
};

template<typename T, size_t SIZE>
Pool<T, SIZE>::Pool(){
    for(size_t i = 0; i < NO_BYTES; ++i){
        info[i] = 0xFF;
    }

    free_element = SIZE;
}

template<typename T, size_t SIZE>
T* Pool<T, SIZE>::alloc() {
    if (pos < SIZE) {
        for (size_t i = pos; i < SIZE; ++i) {
            if (testBit(i)) {
                clrBit(i);
                --free_element;
                ++pos;
                return reinterpret_cast<T*>(elements) + i;
            }
        }
    }

    pos = 0;
    for (size_t i = pos; i < SIZE; ++i) {
        if (testBit(i)) {
            clrBit(i);
            --free_element;
            pos = i + 1;
            return reinterpret_cast<T*>(elements) + i;
        }
    }
    std::cout << free_element << "\n";
    return nullptr;
}


template<typename T, size_t SIZE>
void Pool<T, SIZE>::free(T* p){
    /*if(p == nullptr){
        return;
    }*/

    size_t index = p - reinterpret_cast<T*>(elements);

    if(index >= SIZE){
        return;
    }
    free_element++;
    setBit(index);
    pos = index;
}

template<typename T, size_t SIZE>
int Pool<T, SIZE>::size(){
    return free_element;
}

template <typename T, size_t SIZE>
bool Pool<T, SIZE>::testBit(size_t bit_index) {
  if (bit_index >= SIZE)
    return false;

  size_t byte_offset = bit_index / BITS_IN_UINT8; // Визначаємо, в якому байті знаходиться біт
  size_t bit_pos = bit_index % BITS_IN_UINT8;     // Залишок після ділення — позиція біта в байті

  // Перевіряємо, чи встановлений біт
  return (info[byte_offset] & (1 << bit_pos)) != 0;
}

template <typename T, size_t SIZE>
void Pool<T, SIZE>::setBit(size_t bit_index) {
  if (bit_index >= SIZE)
    return;

  size_t byte_offset = bit_index / BITS_IN_UINT8; // Визначаємо, в якому байті знаходиться біт
  size_t bit_pos = bit_index % BITS_IN_UINT8;     // Залишок після ділення — позиція біта в байті

  // Встановлюємо біт
  info[byte_offset] |= (1 << bit_pos);
}

template <typename T, size_t SIZE>
void Pool<T, SIZE>::clrBit(size_t bit_index) {
  if (bit_index >= SIZE)
    return;

  size_t byte_offset = bit_index / BITS_IN_UINT8; // Визначаємо, в якому байті знаходиться біт
  size_t bit_pos = bit_index % BITS_IN_UINT8;     // Залишок після ділення — позиція біта в байті

  // Очищаємо біт
  info[byte_offset] &= ~(1 << bit_pos);
}



#endif