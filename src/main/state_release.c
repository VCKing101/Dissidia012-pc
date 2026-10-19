#include "main/state_prefixes.h"

extern MainTask *func_088823C4(unsigned int, unsigned int, int,
                              MainTaskCallback);
/* Task/callback forwarding, not release. The native callee consumes a1-a3
 * and returns its task pointer in v0. The observed label argument is unused.
 * Explicitly forward every consumed argument; incidental preserved registers
 * in the old one-argument declaration were insufficient source semantics. */
MainTask *func_08865088(MainStatePrefix *self, unsigned int first,
                        int second, MainTaskCallback update,
                        const void *unused_label) {
    return func_088823C4(self->field_0000, first, second, update);
}
