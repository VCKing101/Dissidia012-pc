#ifndef MAIN_TASK_ITERATION_H
#define MAIN_TASK_ITERATION_H
#include "main/task_runtime.h"
struct MainTaskIteration {
    static MainTask *next(MainTaskOwner *owner, MainTask *cursor) {
        return !cursor ? owner->field_0004 : cursor->field_0034;
    }
    static MainTask *matching_next(MainTaskOwner *owner, MainTask *cursor,
                                   unsigned int mask, unsigned int value) {
        MainTask *result;
        while ((cursor = (result = next(owner,cursor)))) {
            if ((cursor->field_0028 & mask) == value) break;
        }
        return result;
    }
};
#endif
