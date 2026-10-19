#include "main/task_runtime.h"

MainTask *func_088822BC(MainTask *self, MainTaskOwner *owner,
                             unsigned int first, int second,
                             MainTaskCallback update) {
    self->field_0000 = update;
    self->field_0024 = owner;
    self->field_0028 = first;
    self->field_002C = second;
    self->field_0030 = 0;
    self->field_003C = 0;
    self->field_0040 = 0;
    return self;
}
