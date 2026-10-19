#include "main/task_runtime.h"
MainTask::MainTask(MainTaskOwner *owner, unsigned int first, int second,
                   MainTaskCallback update) {
    field_0000 = update;
    field_0024 = owner;
    field_0028 = first;
    field_002C = second;
    field_0030 = 0;
    field_003C = 0;
    field_0040 = 0;
}
