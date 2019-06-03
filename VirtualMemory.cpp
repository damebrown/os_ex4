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
        if (i != current_frame)
        {
            bool is_good = true;
            for (int j = 0; j < PAGE_SIZE - 1; ++j)
            {
                auto addr = uint64_t((i * PAGE_SIZE) + j);
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

uint64_t get_cyclic_distance(uint64_t p1, uint64_t p2)
{
    uint64_t _min = (uint64_t) min((int) NUM_PAGES - abs((int) (p1 - p2)), abs((int) (p1 - p2)));
    return _min;
}

/**
 * mdp is maximal distance page.
 * @param frame_num
 * @param depth
 * @param p_num
 * @param p_max
 * @param p_insert
 * @return
 */
uint64_t find_mdp(int frame_num, int depth, uint64_t p_num, uint64_t p_max, uint64_t p_insert)
{
    if (depth == 0)
    {
        return p_num;
    }
    int val = 0;
    depth = depth - 1;
    for (int i = 0; i < PAGE_SIZE - 1; ++i)
    {
        auto addr = uint64_t((frame_num * PAGE_SIZE) + i);
        PMread(addr, &val);
        if (val)
        {
            uint64_t new_p = find_mdp(val, depth, p_num +(int)pow(val, depth), p_max, p_insert);
            if (get_cyclic_distance(new_p, p_insert) >
                get_cyclic_distance(p_max, p_insert))
            {
                p_max = new_p;
            }
        }
    }
    return p_max;
}

/**
 * Update the table!
 * @param remainder
 * @param directions_table
 */
void update_directions_table(uint64_t remainder, uint64_t *directions_table)
{
    for (int i = TABLES_DEPTH - 1; i >= 0; --i)
    {
        directions_table[i] = remainder % PAGE_SIZE;
        remainder = remainder / PAGE_SIZE;
    }
}

int delete_page(uint64_t page_to_delete)
{
    uint64_t directions_table[TABLES_DEPTH] = {0};
    update_directions_table(page_to_delete, directions_table);
    word_t frame_i = 0;
    for (int i = TABLES_DEPTH - 1; i >= 0; --i)
    {
        word_t prev_frame_i = frame_i;
        uint64_t t = (prev_frame_i) * PAGE_SIZE + (uint64_t) i;
        PMread(t, &frame_i);
    }
    PMevict((uint64_t) frame_i, page_to_delete);
    return frame_i;
}


int find_physical_address(uint64_t page_number, bool if_read)
{
    bool build = false;
    uint64_t directions_table[TABLES_DEPTH] = {0};
    update_directions_table(page_number, directions_table);
    word_t frame_i = 0;
    for (uint64_t i = 0; i < TABLES_DEPTH; ++i)
    {
        word_t prev_frame_i = frame_i;
        uint64_t t = (prev_frame_i) * PAGE_SIZE + i;
        PMread(t, &frame_i);
        if (!frame_i)
        {
            build = true;
            word_t empty_frame = find_empty_frame(prev_frame_i);
            // if there are no empty frames, empty_frame = -1
            if (empty_frame < 0)
            {
                uint64_t p_delete = find_mdp(0, TABLES_DEPTH, 0, 0, page_number);
                empty_frame = delete_page(p_delete);
            }
            PMwrite((prev_frame_i) * PAGE_SIZE + i, empty_frame);
            frame_i = empty_frame;
        }
    }
    if (build && if_read)
    {
        PMrestore((uint64_t) frame_i, page_number);
    }
    return frame_i;
}

void VMinitialize()
{
    clearTable(0);
}


uint64_t get_offset(uint64_t virtualAddress)
{
    return virtualAddress % PAGE_SIZE;
}

int get_address(uint64_t virtualAddress, bool if_read = false)
{
    uint64_t addr_remainder = virtualAddress / PAGE_SIZE;
    return find_physical_address(addr_remainder, if_read);
}

int VMread(uint64_t virtualAddress, word_t *value)
{
    PMread((get_address(virtualAddress, true)) * PAGE_SIZE + get_offset(virtualAddress), value);
    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value)
{
    PMwrite((get_address(virtualAddress)) * PAGE_SIZE + get_offset(virtualAddress), value);
    return 1;
}
