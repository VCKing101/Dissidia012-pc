#include "main/task_runtime.h"

typedef struct MainTaskCreateFrame {
    MainTaskCallback update;
    int priority;
    unsigned int mask;
} MainTaskCreateFrame;

/* Probe only: explicitly represent the observed automatic argument stores
 * and reloads. This does not establish original volatile qualifications. */
MainTask *func_088823C4(unsigned int owner_word, unsigned int first,
                       int second, MainTaskCallback update) {
    MainTask *result;
    {
    MainTaskCreateFrame arguments = {update,second,first};
    MainTaskOwner *owner = (MainTaskOwner *)owner_word;
    MainTask *allocated = func_0885C6CC(owner->field_0000,0x4C,0,0,1,0);
    MainTaskCallback saved_update = arguments.update;
    int saved_priority = arguments.priority;
    unsigned int saved_mask = arguments.mask;
    if (allocated) {
        func_088822BC(allocated,owner,saved_mask,saved_priority,saved_update);
        result = allocated;
    } else {
        result = 0;
    }
    }
    func_08882790((MainTaskOwner *)owner_word,result);
    return result;
}
