#include "main/agent4_types.h"

extern "C" {
extern int D_089C78D0;

/* Deleting destructor; vtable 0x089C78D0. */
void func_08828B9C(Agent4Vptr08 *self, unsigned int flags) {
    Agent4Vptr08::destroy(self, flags, &D_089C78D0);
}

}
