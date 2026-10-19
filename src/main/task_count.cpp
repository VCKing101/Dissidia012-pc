#include "main/task_iteration.h"
extern "C" int func_088826D4(MainTaskOwner *owner) {
    int count = 0;
    MainTask *cursor = 0;
    while ((cursor = MainTaskIteration::next(owner,cursor))) count++;
    return count;
}

