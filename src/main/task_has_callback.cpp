#include "main/task_iteration.h"
extern "C" int func_0888274C(MainTaskOwner *owner, MainTaskCallback update) {
    MainTask *cursor = 0;
    while ((cursor = MainTaskIteration::next(owner,cursor))) {
        if (cursor->field_0000 == update) return 1;
    }
    return 0;
}
