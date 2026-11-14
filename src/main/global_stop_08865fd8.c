#include "main/agent2_types.h"

extern Agent2GlobalState *D_089D5EC0;
extern void func_08865E50(Agent2GlobalState *);

void func_08865FD8(void) {
    if (D_089D5EC0->field_2CD0) return;
    func_08865E50(D_089D5EC0);
}
