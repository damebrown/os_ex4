#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <cmath>
#include <bitset>
#include <iostream>


void clearTable(uint64_t frameIndex)
{
    for (uint64_t i = 0; i < PAGE_SIZE; ++i)
    {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

int find_empty_frame(){
    int val = 0;

    for (int i=0; i<RAM_SIZE; ++i){
        bool is_good = true;
        for (int j = 0; j < PAGE_SIZE ; ++j){
            PMread(uint64_t (i * PAGE_SIZE + j), &val);
            if (val){
                is_good = false;
                break;
            }
        }
        if(is_good){
            return i;
        }
    }
    return -1;
}

//int find_physical_address(uint64_t remainder)
//{
//    uint64_t directions_table[TABLES_DEPTH] = {0};
//    for (int i = TABLES_DEPTH - 1; i >= 0; --i)
//    {
//        directions_table[i] = remainder % PAGE_SIZE;
//        remainder = remainder / PAGE_SIZE;
//    }
//    std::cout << "here" << std::endl;
//    word_t val = 0;
//    for (int i = 0; i < TABLES_DEPTH; ++i)
//    {
//        PMread((val) * PAGE_SIZE + directions_table[i], &val);
//        if (!val){
//            find_empty_frame();
//        }
//    }
//    return 0;
//}

void VMinitialize()
{
    clearTable(0);
}


int VMread(uint64_t virtualAddress, word_t *value)
{
    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value)
{
    uint64_t copy = virtualAddress;
    uint64_t offset = virtualAddress % PAGE_SIZE;
    uint64_t x = virtualAddress / PAGE_SIZE;
    for (int i = 0; i < TABLES_DEPTH; ++i)
    {
        word_t *next = nullptr;
        //PMread(x,next);


    }
    return 1;
}
