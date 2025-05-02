#include <iostream>
#include <list>
#include <chrono>

#include "pool2.h"
#include "allocator.h"


void test_my_alloc(){
    std::chrono::duration<double> res;
    for(int k = 0; k < 200; ++k){
    auto start = std::chrono::high_resolution_clock::now();
    
    std::list<int, MyLib::Allocator<int>> a;
    for(int i = 0; i < 1000000; ++i){
        a.push_back(i);
    }
    auto it = a.begin();
    for (int i = 0; i < 10000 && it != a.end(); ++i) {
        it = a.erase(it);
    }
    
    for(int i = 0; i < 10000; ++i){
        a.push_back(i);
    }
    res += (std::chrono::high_resolution_clock::now() - start);
    }
    std::cout <<"My allocator: " << res.count() << "\n";
}

void test_stl_alloc(){
    std::chrono::duration<double> res;
    for(int k = 0; k < 200; ++k){
    auto start = std::chrono::high_resolution_clock::now();
    
    std::list<int, std::allocator<int>> a;
    for(int i = 0; i < 1000000; ++i){
        a.push_back(i);
    }
    auto it = a.begin();
    for (int i = 0; i < 10000 && it != a.end(); ++i) {
        it = a.erase(it);
    }
    
    for(int i = 0; i < 10000; ++i){
        a.push_back(i);
    }
    res += (std::chrono::high_resolution_clock::now() - start);
    }
    std::cout <<"Stl allocator: " << res.count() << "\n";
}


int main(){
    test_my_alloc();
    test_stl_alloc();
    return 0;
}