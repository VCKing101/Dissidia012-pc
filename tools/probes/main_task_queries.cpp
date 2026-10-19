#include "main_task_iteration.hpp"
extern "C" int func_088826D4(MainTaskOwner *owner) {
    int count = 0;
    MainTask *cursor = 0;
    while ((cursor = MainTaskIteration::next(owner,cursor))) count++;
    return count;
}

extern "C" int func_0888274C(MainTaskOwner *owner, MainTaskCallback update) {
    MainTask *cursor = 0;
    while ((cursor = MainTaskIteration::next(owner,cursor))) {
        if (cursor->field_0000 == update) return 1;
    }
    return 0;
}
