#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <cmath>
#include <bitset>
#include <iostream>

using namespace std;

void clearTable(uint64_t frameIndex)
{
    for (uint64_t i = 0; i < PAGE_SIZE; ++i)
    {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

/**
 * this function finds an empty frame in the physical memory.
 * @return if i is the index of the first free frame, the function returns i
 */
word_t find_empty_frame(int current_frame)
{
    int val = 0;
    for (word_t i = 1; i < RAM_SIZE - 1; ++i)
    {
        if (i != current_frame){
            bool is_good = true;
            for (int j = 0; j < PAGE_SIZE - 1; ++j)
            {
                auto addr = uint64_t ((i * PAGE_SIZE) + j);
                if (addr >= RAM_SIZE)
                {
                    return -1;
                }
                PMread(addr, &val);
                if (val)
                {
                    is_good = false;
                    break;
                }
            }
            if (is_good)
            {
                return i;
            }
        }
    }
    return -1;
}

int find_physical_address(uint64_t remainder)
{
    uint64_t directions_table[TABLES_DEPTH] = {0};
    for (int i = TABLES_DEPTH - 1; i >= 0; --i)
    {
        directions_table[i] = remainder % PAGE_SIZE;
        remainder = remainder / PAGE_SIZE;
    }
    word_t frame_i = 0;
    word_t print = 0;
    for (int i = 0; i < TABLES_DEPTH; ++i)
    {
        word_t prev_frame_i = frame_i;
        uint64_t t = (prev_frame_i) * PAGE_SIZE + directions_table[i];
        PMread(t, &frame_i);
        if (!frame_i)
        {
            word_t empty_frame = find_empty_frame(prev_frame_i);
            if (empty_frame < 0)
            {
                //TODO start deleting with cyclic distance
                cout << "empty frame not found" << endl;
            }
            else
            {
                PMwrite((prev_frame_i) * PAGE_SIZE + directions_table[i], empty_frame);
//                cout<<"\treading from address="<<(prev_frame_i)*PAGE_SIZE+directions_table[i]<<endl;
                PMread((prev_frame_i) * PAGE_SIZE + directions_table[i], &print);
                cout<<"frame: "<<prev_frame_i<<", Line:"<<directions_table[i]<<", val:"<<print<<endl;
                frame_i = empty_frame;
            }
        }
    }
    return frame_i;
}

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
    uint64_t offset = virtualAddress % PAGE_SIZE;
    uint64_t addr_remainder = virtualAddress / PAGE_SIZE;
    int address = find_physical_address(addr_remainder);
    PMwrite((address) * PAGE_SIZE + offset, value);
    word_t print = 0;
//    cout << "\t\t\tReading from  address = " << (address) * PAGE_SIZE + offset << endl;
    PMread((address) * PAGE_SIZE + offset, &print);
    cout << "frame: " << address << " , Line (offset): " << offset << " , val: " << print << endl;
    return 1;
}
