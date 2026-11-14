#include "main/agent2_types.h"

extern void func_08865710(Agent2Table *);
extern void func_08992F04(void *);

void func_08865488(Agent2Table *self, unsigned int flags) {
    if (self) {
        func_08865710(self);
        if (flags & 1) func_08992F04(self);
    }
}
