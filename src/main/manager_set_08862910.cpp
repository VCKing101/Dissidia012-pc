#include "main/agent10_types.h"

extern "C" {
extern Agent10Root *D_089D5EA0;
int func_08867984(int key, int flags);

void func_08862910(int key, int a, int b, unsigned char on) {
    int entry = func_08867984(key, 0);
    D_089D5EA0->manager->set05(entry, a, b, on);
}
}
