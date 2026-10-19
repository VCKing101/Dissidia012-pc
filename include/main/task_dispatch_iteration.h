#ifndef MAIN_TASK_DISPATCH_ITERATION_H
#define MAIN_TASK_DISPATCH_ITERATION_H

#include "main/task_iteration.h"

/* Advance the mutable pending cursor and publish it before invoking a task.
 * The dispatcher rechecks the cursor when a callback changes the filter. */
struct MainTaskPendingIteration {
    static void advance(MainTaskOwner *owner, MainTask *&cursor,
                        unsigned int mask, unsigned int value) {
        while ((cursor = MainTaskIteration::next(owner, cursor))) {
            if ((cursor->field_0028 & mask) == value) break;
        }
        owner->field_0014 = cursor;
    }
};

#endif
