#include "VirtualMemory.h"
#include "PhysicalMemory.h"

#include <cstdio>
#include <cassert>
#include <iostream>

using namespace std;



int main(int argc, char **argv) {
    VMinitialize();
//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        printf("writing to %llu\n", (long long int) i);
//        VMwrite(5 * i * PAGE_SIZE, i);
//    }
//
//    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        word_t value;
//        VMread(5 * i * PAGE_SIZE, &value);
//        printf("reading from %llu %d\n", (long long int) i, value);
//        assert(uint64_t(value) == i);
//    }
//    printf("success\n");
//
//    return 0;
//    for (uint64_t i = 0; i < 1025; ++i){
//        cout << i<<endl;
//    }
    VMwrite(15, 6);
    VMwrite(15, 6);
}