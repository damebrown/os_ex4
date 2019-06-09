#include "VirtualMemory.h"
#include "PhysicalMemory.h"
//#include "tests/FlowExampleSimulation.cpp"
#include <cmath>
#include <bitset>
#include <iostream>
#include <vector>


void printSubTree(uint64_t root, int depth, bool isEmptyMode)
{
    if (depth == TABLES_DEPTH)
    {
        return;
    }
    word_t currValue = 0;

    if ((isEmptyMode || root == 0) && depth != 0)
    {
        isEmptyMode = true;
    }

    //right son
    PMread(root * PAGE_SIZE + 1, &currValue);
    printSubTree(static_cast<uint64_t>(currValue), depth + 1, isEmptyMode);

    //father
    for (int _ = 0; _ < depth; _++)
    {
        std::cout << '\t';
    }
    if (isEmptyMode)
    {
        std::cout << '_' << '\n';
    } else
    {
        if (depth == TABLES_DEPTH - 1)
        {
            word_t a, b;
            PMread(root * PAGE_SIZE + 0, &a);
            PMread(root * PAGE_SIZE + 1, &b);
            std::cout << root << " -> (" << a << ',' << b << ")\n";
        } else
        {
            std::cout << root << '\n';
        }
    }

    //left son
    PMread(root
           * PAGE_SIZE + 0, &currValue);
    printSubTree(static_cast <uint64_t>(currValue), depth + 1, isEmptyMode);
}

/**
 * print's the virtual memory tree. feel free to use this function is Virtual Memory for debuging.
 */
void printTree()
{
    std::cout << "---------------------" << '\n';
    std::cout << "Virtual Memory:" << '\n';
    printSubTree(0, 0, false);
    std::cout << "---------------------" << '\n';
}

/**
 * get a copy of the current physical memory
 */
std::vector<word_t> getRam()
{
    std::vector<word_t> RAM(RAM_SIZE);
    word_t tempWord;
    for (uint64_t i = 0; i < NUM_FRAMES; i++)
    {
        for (uint64_t j = 0; j < PAGE_SIZE; j++)
        {
            PMread(i * PAGE_SIZE + j, &tempWord);
            RAM[i * PAGE_SIZE + j] = tempWord;
        }
    }
    return RAM;
}

static bool printType = false; // You may change this to True if you want.

/**
 * print the current state of the pysical memory. feel free to use this function is Virtual Memory for debuging.
 */
void printRAM()
{
    std::cout << "---------------------" << '\n';
    std::cout << "Physical Memory:" << '\n';
    std::vector<word_t> RAM = getRam();

    if (printType)
    {
        for (uint64_t i = 0; i < NUM_FRAMES; i++)
        {
            std::cout << "frame " << i << ":\n";
            for (uint64_t j = 0; j < PAGE_SIZE; j++)
            {
                std::cout << "(" << j << ") " << RAM[i * PAGE_SIZE + j] << "\n";
            }
            std::cout << "-----------" << "\n";
        }
    } else
    {

        std::cout << "FRAME INDICES -\t";
        for (uint64_t i = 0; i < NUM_FRAMES; i++)
        {
            std::cout << "F" << i << ": (";
            for (uint64_t j = 0; j < PAGE_SIZE - 1; j++)
            {
                std::cout << j << ",\t";
            }
            std::cout << PAGE_SIZE - 1 << ")\t";
        }
        std::cout << '\n';
        std::cout << "DATA -\t\t\t";
        for (uint64_t i = 0; i < NUM_FRAMES; i++)
        {
            std::cout << "     ";
            for (uint64_t j = 0; j < PAGE_SIZE - 1; j++)
            {
                std::cout << RAM[i * PAGE_SIZE + j] << ",\t";
            }
            std::cout << RAM[i * PAGE_SIZE + PAGE_SIZE - 1] << " \t";
        }
        std::cout << '\n';
    }

    std::cout << "---------------------" << '\n';
}


using namespace std;
//
void clearTable(uint64_t frameIndex)
{
    for (uint64_t i = 0; i < PAGE_SIZE; ++i)
    {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

bool is_empty_frame(int frame)
{
    bool empty = true;
    int val = 0;
    for (int j = 0; j < PAGE_SIZE; ++j)
    {
        auto addr = uint64_t((frame * PAGE_SIZE) + j);
        PMread(addr, &val);
        if (val)
        {
            empty = false;
            break;
        }
    }
    return empty;
}

//int if_leaf(int frame, int depth, int check_f, int sum)
//{
//    if ((!depth))
//    {
//        if (check_f == frame)
//        {
//            return 1;
//        }
//        return 0;
//    }
//    for (int i = 0; i < PAGE_SIZE; ++i)
//    {
//        int next;
//        PMread(check_f * PAGE_SIZE + i, &next);
//        if (next)
//        {
//            sum += if_leaf(frame, depth - 1, next, sum);
//        }
//    }
//    return sum;
//}

int if_leaf(int offs, int frame, int depth, int check_f, int prev_check, int sum)
{
    if ((!depth))
    {
        if (check_f == frame)
        {
            return 1;
        }
        return 0;
    }
    if (check_f == frame)
    {
        PMwrite(prev_check*PAGE_SIZE+offs, 0);
        //cout<<"--____________________________________--"<<endl;
        //cout<<"$$$   this frame is: "<< check_f << "pointer frame is"<< prev_check << " at offs" << offs <<endl;
    }
    for (int i = 0; i < PAGE_SIZE; ++i)
    {
        offs = i;
        int next;
        PMread(check_f * PAGE_SIZE + i, &next);
        if (next)
        {
            sum +=(if_leaf(offs,frame, depth - 1, next, check_f, sum));

        }
    }
    return sum;
}

/**
 * this function finds an empty frame in the physical memory.
 * @return if i is the index of the first free frame, the function returns i
 */
word_t find_empty_frame(int current_frame)
{
    for (word_t i = 1; i < NUM_FRAMES; ++i)
    {
        if (i != current_frame)
        {
            if (is_empty_frame(i))
            {
                if (if_leaf(0, i, TABLES_DEPTH, 0, 0, 0) == 0)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

uint64_t cyclic_distance(uint64_t p1, uint64_t p2)
{
    uint64_t _min = (uint64_t) min((int) NUM_PAGES - abs((int) (p1 - p2)), abs((int) (p1 - p2)));
    return _min;
}

uint64_t find_mdp(int p2insert, int depth, int frame, uint64_t mdp_option, uint64_t max_mdp,
        uint64_t count)
{
    if (!depth)
    {
        if (cyclic_distance(mdp_option, p2insert) > cyclic_distance(max_mdp, p2insert))
        {
            return mdp_option;
        }
        return p2insert;
    }
    int content = 0;
    for (int i = 0; i < PAGE_SIZE; ++i)
    {
        PMread(frame * PAGE_SIZE + i, &content);
        if (content)
        {
            uint64_t new_branch = count + i * (uint64_t) pow((OFFSET_WIDTH + 1), (depth - 1));
            if (content <= NUM_FRAMES){
                mdp_option = find_mdp(p2insert, depth - 1, content, new_branch, max_mdp, new_branch);
            }
            if (cyclic_distance(mdp_option, p2insert) > cyclic_distance(max_mdp, p2insert))
            {
                max_mdp = mdp_option;
            }
        }
    }
    return max_mdp;

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

//-------------------------------------------
int
recursion_delete(int index, uint64_t directions_table[], int depth, int page_to_delete, int frame,
                 bool if_leaf, int prev_frame)
{
    if (depth == 0)
    {
        if (if_leaf && frame)
        {
            cout << "====EVICTIN==== frame " << frame << ", page number " <<
            page_to_delete<<endl;
            PMevict((uint64_t) frame, page_to_delete);
            clearTable(frame);
        }
        if (is_empty_frame(frame)){
            PMwrite(prev_frame * PAGE_SIZE + directions_table[index - 1], 0);
//            if (frame){ return frame; }
            if (!frame){ cout << "FRAME IS 0 !!!!!" << endl;}
            return frame;
        }
        PMwrite(prev_frame * PAGE_SIZE + directions_table[index - 1], 0);
//        if (frame){ return frame; }
        if (!frame){ cout << "FRAME IS 0 !!!!!" << endl;}
        return frame;
    }
    int _prev_frame = frame;
    PMread(frame * PAGE_SIZE + directions_table[index], &frame);
    return recursion_delete(index + 1, directions_table, depth - 1, page_to_delete, frame,
            if_leaf, _prev_frame);
}

int new_delete_page(uint64_t p2delete, bool evict, int depth)
{
    uint64_t directions_table[TABLES_DEPTH] = {0};
    update_directions_table(p2delete, directions_table);
    if (!evict)
    {
        int page = recursion_delete(0, directions_table, TABLES_DEPTH, p2delete,
                0, true, 0);
        return page;
    }
    else
    {
        int page = recursion_delete(0, directions_table, depth, p2delete, 0,
                false, 0);
        return page;
    }
}


void update_ptr(word_t empty_frame){
    word_t word = 0;
    for (uint64_t  frame = 0; frame < NUM_FRAMES; frame++){
        if (frame != empty_frame){
            for (uint64_t offset = 0; offset < PAGE_SIZE; ++offset){
                PMread(frame * PAGE_SIZE + offset, &word);
                if (word == empty_frame){
                    PMwrite(frame * PAGE_SIZE + offset, 0);
                }
            }
        }
    }
}

int find_physical_address(uint64_t page_number, bool if_read)
{
    bool build = false, evict = false;
    uint64_t directions_table[TABLES_DEPTH] = {0};
    update_directions_table(page_number, directions_table);
    word_t frame_i = 0;
    uint64_t p_delete = 0;
    int depth = TABLES_DEPTH;
    word_t word = 0;
    for (uint64_t i = 0; i < TABLES_DEPTH; ++i)
    {
        word_t prev_frame_i = frame_i;
        uint64_t addr = (prev_frame_i) * PAGE_SIZE + directions_table[i];
        if (addr >= RAM_SIZE)
        {
            cout << "THIS ONE!!:" << addr << endl;
            return -1;
        }

        printRAM();
        printTree();
        PMread(addr, &frame_i);
        printRAM();
        printTree();
        if (!frame_i)
        {
            printTree();
            word_t empty_frame = find_empty_frame(prev_frame_i);
            // if there are no empty frames, empty_frame = -1
            if (empty_frame < 0)
            {
                if (i == TABLES_DEPTH - 1){
                    evict = false;
                    depth = TABLES_DEPTH;
                }
                    p_delete = find_mdp(page_number, TABLES_DEPTH, 0, 0, page_number, 0);
                if ((!evict))
                {
                    printTree();
                    printTree();
                }
                printTree();
                empty_frame = new_delete_page(p_delete, evict, depth);
                evict = true;
                depth--;
            }
            printTree();
            PMwrite((prev_frame_i) * PAGE_SIZE + directions_table[i], empty_frame);
            printTree();
            frame_i = empty_frame;
        }
    }
    if (if_read)
    {
        cout << "===========RESTORING========= frame " << frame_i << ", page number " << page_number<<endl;
        PMrestore((uint64_t) frame_i, page_number);
    }
    else{
        cout << "===========WRITING========= frame " << frame_i << ", page number " <<
        page_number<<endl;
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

int get_address(uint64_t virtualAddress, bool if_read)
{
    uint64_t addr_remainder = (uint64_t) ((int) virtualAddress / PAGE_SIZE);
//    if (if_read)
//    {
//        cout << "===========READING========= page number " << addr_remainder << ", offset " <<
//        (virtualAddress % PAGE_SIZE) << endl;
//    }
//    else{
//        cout << "===========WRITING========= page number " << addr_remainder << ", offset " <<
//             (virtualAddress % PAGE_SIZE) << endl;
//    }
    int addr = find_physical_address(addr_remainder, if_read);
    if (addr < RAM_SIZE && addr >= 0){ return addr;}
    return -1;
}

int VMread(uint64_t virtualAddress, word_t *value)
{
    int addr = get_address(virtualAddress, true);
    if (addr < 0){ return 0; }
    PMread((addr) * PAGE_SIZE + get_offset(virtualAddress), value);
    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value)
{
    int addr = get_address(virtualAddress, true);
    if (addr < 0){ return 0; }
    PMwrite((addr) * PAGE_SIZE + get_offset(virtualAddress), value);
    return 1;
}
