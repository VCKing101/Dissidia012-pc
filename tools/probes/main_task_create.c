#include "main/task_runtime.h"

MainTask *func_088823C4(unsigned int owner_word, volatile unsigned int first,
                    volatile int second, MainTaskCallback volatile update) {
    MainTaskOwner *owner = (MainTaskOwner *)owner_word;
    MainTask *allocated = func_0885C6CC(owner->field_0000, 0x4C, 0, 0, 1, 0);
    MainTask *result = 0;
    if (allocated) {
        func_088822BC(allocated, owner, first, second, update);
        result = allocated;
    }
    func_08882790(owner, result);
    return result;
}
