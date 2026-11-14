#include "main/agent4_types.h"

extern "C" {
extern int D_089C6D70;
extern int D_089C6D80;

/* Deleting destructor; vtable 0x089C6D70. */
void func_088104B8(Agent4Vptr08 *self, unsigned int flags) {
    Agent4Vptr08::destroy(self, flags, &D_089C6D70);
}

/* Deleting destructor; vtable 0x089C6D80. */
void func_088104EC(Agent4Vptr08 *self, unsigned int flags) {
    Agent4Vptr08::destroy(self, flags, &D_089C6D80);
}

}
