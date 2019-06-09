#include "VirtualMemory.h"
#include "PhysicalMemory.h"

#include <cstdio>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

//
///*
// * helper for printTree
// */
//void printSubTree(uint64_t root, int depth, bool isEmptyMode)
//{
//    if (depth == TABLES_DEPTH)
//    {
//        return;
//    }
//    word_t currValue = 0;
//
//    if ((isEmptyMode || root == 0) && depth != 0)
//    {
//        isEmptyMode = true;
//    }
//
//    //right son
//    PMread(root * PAGE_SIZE + 1, &currValue);
//    printSubTree(static_cast<uint64_t>(currValue), depth + 1, isEmptyMode);
//
//    //father
//    for (int _ = 0; _ < depth; _++)
//    {
//        std::cout << '\t';
//    }
//    if (isEmptyMode)
//    {
//        std::cout << '_' << '\n';
//    } else
//    {
//        if (depth == TABLES_DEPTH - 1)
//        {
//            word_t a, b;
//            PMread(root * PAGE_SIZE + 0, &a);
//            PMread(root * PAGE_SIZE + 1, &b);
//            std::cout << root << " -> (" << a << ',' << b << ")\n";
//        } else
//        {
//            std::cout << root << '\n';
//        }
//    }
//
//    //left son
//    PMread(root
//           * PAGE_SIZE + 0, &currValue);
//    printSubTree(static_cast <uint64_t>(currValue), depth + 1, isEmptyMode);
//}
//
///**
// * print's the virtual memory tree. feel free to use this function is Virtual Memory for debuging.
// */
//void printTree()
//{
//    std::cout << "---------------------" << '\n';
//    std::cout << "Virtual Memory:" << '\n';
//    printSubTree(0, 0, false);
//    std::cout << "---------------------" << '\n';
//}

///**
// * get a copy of the current physical memory
// */
//std::vector<word_t> getRam()
//{
//    std::vector<word_t> RAM(RAM_SIZE);
//    word_t tempWord;
//    for (uint64_t i = 0; i < NUM_FRAMES; i++)
//    {
//        for (uint64_t j = 0; j < PAGE_SIZE; j++)
//        {
//            PMread(i * PAGE_SIZE + j, &tempWord);
//            RAM[i * PAGE_SIZE + j] = tempWord;
//        }
//    }
//    return RAM;
//}
//
//static bool printType = false; // You may change this to True if you want.
//
///**
// * print the current state of the pysical memory. feel free to use this function is Virtual Memory for debuging.
// */
//void printRAM()
//{
//    std::cout << "---------------------" << '\n';
//    std::cout << "Physical Memory:" << '\n';
//    std::vector<word_t> RAM = getRam();
//
//    if (printType)
//    {
//        for (uint64_t i = 0; i < NUM_FRAMES; i++)
//        {
//            std::cout << "frame " << i << ":\n";
//            for (uint64_t j = 0; j < PAGE_SIZE; j++)
//            {
//                std::cout << "(" << j << ") " << RAM[i * PAGE_SIZE + j] << "\n";
//            }
//            std::cout << "-----------" << "\n";
//        }
//    } else
//    {
//
//        std::cout << "FRAME INDICES -\t";
//        for (uint64_t i = 0; i < NUM_FRAMES; i++)
//        {
//            std::cout << "F" << i << ": (";
//            for (uint64_t j = 0; j < PAGE_SIZE - 1; j++)
//            {
//                std::cout << j << ",\t";
//            }
//            std::cout << PAGE_SIZE - 1 << ")\t";
//        }
//        std::cout << '\n';
//        std::cout << "DATA -\t\t\t";
//        for (uint64_t i = 0; i < NUM_FRAMES; i++)
//        {
//            std::cout << "     ";
//            for (uint64_t j = 0; j < PAGE_SIZE - 1; j++)
//            {
//                std::cout << RAM[i * PAGE_SIZE + j] << ",\t";
//            }
//            std::cout << RAM[i * PAGE_SIZE + PAGE_SIZE - 1] << " \t";
//        }
//        std::cout << '\n';
//    }
//
//    std::cout << "---------------------" << '\n';
//}


int main(int argc, char **argv) {
    VMinitialize();
    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
//        printf("writing %llu to %llu\n", (long long int) i, 5 * i * PAGE_SIZE);
//        VMwrite(5 * i * PAGE_SIZE, i);
        printf("writing %llu to %llu\n", (long long int) i, i);
        VMwrite(i, i);
//        printRAM();
//        printTree();
    }
    cout << "\n\n============READING============\n" << endl;
    for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
        word_t value;
//        VMread(5 * i * PAGE_SIZE, &value);
//        printf("reading: the content in %llu is %d\n", (long long int) 5 * i * PAGE_SIZE, value);
        VMread(i, &value);
        printf("reading: the content in %llu is %d\n", (long long int) i, value);
//        printRAM();
//        printTree();
        assert(uint64_t(value) == i);
    }
    printf("success\n");

    return 0;
}