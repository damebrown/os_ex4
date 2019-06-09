////
//// Created by daniel.brown1 on 6/7/19.
////
//
//#include "MemoryConstants.h"
//#include "PhysicalMemory.h"
//
//word_t find_empty_frame(int current_frame)
//{
////    int val = 0;
////    for (word_t i = 1; i < NUM_FRAMES; ++i)
////    {
////        if (i != current_frame)
////        {
////            bool is_good = true;
////            for (int j = 0; j < PAGE_SIZE; ++j)
////            {
////                auto addr = uint64_t((i * PAGE_SIZE) + j);
////                if (addr >= RAM_SIZE)
////                {
////                    return -1;
////                }
////                PMread(addr, &val);
////                if (val)
////                {
////                    is_good = false;
////                    break;
////                }
////            }
////            if (is_good)
////            {
////                return i;
////            }
////        }
////    }
////    return -1;
//}
//
//word_t _find_empty_frame(int original_frame, word_t current_frame, int depth, uint64_t *
//directions_t, word_t max_frame)
//{
//    if (depth < TABLES_DEPTH)
//    {
//        word_t son;
//        auto son_index = (word_t) directions_t[depth - 1];
//        auto addr = ((current_frame * PAGE_SIZE) + son_index);
//        PMread(addr, &son);
//        if (son != 0)
//        {
//            if (max_frame < son)
//            {
//                max_frame = son;
//            }
//            word_t ret_val = _find_empty_frame(original_frame, son, depth + 1, directions_t, son);
//            if (ret_val > 0)
//            {
//                return ret_val;
//            }
//        }
//        else if (max_frame + 1 < NUM_FRAMES)
//        {
//            return max_frame;
//        }
//        //        else if ((son_index == PAGE_SIZE - 1) && ((current_frame != original_frame) || (!original_frame)))
//        //        {
//        //            return current_frame;
//        //        }
//    }
//    else if (max_frame + 1 < NUM_FRAMES)
//    {
//        return max_frame;
//    }
//    return -1;
//}
// delete
//    uint64_t directions_table[TABLES_DEPTH] = {0};
//    update_directions_table(page_to_delete, directions_table);
//    word_t frame_i = 0;
//    for (int i = TABLES_DEPTH - 1; i >= 0; --i)
//    {
//        word_t prev_frame_i = frame_i;
//        uint64_t t = (prev_frame_i) * PAGE_SIZE + (uint64_t) directions_table[i];
//        PMread(t, &frame_i);
//    }
//    PMevict((uint64_t) frame_i, page_to_delete);
//    return fra
//
// me_i;

//void print(word_t &val){
//    PMread(0,&val);
//    cout << "frame 0: 0 = " << val << endl;
//    PMread(1,&val);
//    cout << "frame 0: 1 = " << val << endl;
//    PMread(2,&val);
//    cout << "frame 1: 0 = " << val << endl;
//    PMread(3,&val);
//    cout << "frame 1: 1 = " << val << endl;
//    PMread(4,&val);
//    cout << "frame 2: 0 = " << val << endl;
//    PMread(5,&val);
//    cout << "frame 2: 1 = " << val << endl;
//    PMread(6,&val);
//    cout << "frame 3: 0 = " << val << endl;
//    PMread(7,&val);
//    cout << "frame 3: 1 = " << val << endl;
//    PMread(8,&val);
//    cout << "frame 4: 0 = " << val << endl;
//    PMread(9,&val);
//    cout << "frame 4: 1 = " << val << endl;
//    PMread(10,&val);
//    cout << "frame 5: 0 = " << val << endl;
//    PMread(11,&val);
//    cout << "frame 5: 1 = " << val << endl;
//    PMread(12,&val);
//    cout << "frame 6: 0 = " << val << endl;
//    PMread(13,&val);
//    cout << "frame 6: 1 = " << val << endl;
//    PMread(14,&val);
//    cout << "frame 7: 0 = " << val << endl;
//    PMread(15,&val);
//    cout << "frame 7: 1 = " << val << endl<< endl;
//}