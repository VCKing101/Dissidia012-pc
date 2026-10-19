#include "main/task_dispatch_iteration.h"

extern "C" MainTask *D_089D69B0;

extern "C" void func_0888245C(MainTaskOwner *owner, unsigned int first,
                             unsigned int second) {
    unsigned int mask = first;
    unsigned int value = second;
    MainTask *task = MainTaskIteration::matching_next(owner, 0, mask, value);
    while (task) {
        MainTask *next = task;
        MainTaskPendingIteration::advance(owner, next, mask, value);
        {
            MainTask *saved = D_089D69B0;
            D_089D69B0 = task;
            owner->field_0010 = task;
            if (!task->field_0000) func_088828BC(owner, task);
            else {
                task->field_0044 = mask;
                MainTaskCallback update = task->field_0000;
                task->field_0048 = value;
                update(task);
                mask = task->field_0044;
                value = task->field_0048;
            }
            D_089D69B0 = saved;
            owner->field_0010 = 0;
        }
        task = owner->field_0014;
        if (task && (task->field_0028 & mask) != value) {
            MainTaskPendingIteration::advance(owner, task, mask, value);
        }
    }
}
