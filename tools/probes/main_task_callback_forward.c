#include "main/state_prefixes.h"
#include "main/callback_runtime.h"
extern void *func_088823C4(unsigned int, unsigned int, unsigned int, MainRegisteredCallback);
void *func_08865088(MainStatePrefix *self, unsigned int first, unsigned int second,
                    MainRegisteredCallback update, const void *unused_label) {
    return func_088823C4(self->field_0000, first, second, update);
}
