#include "main/agent7_types.h"

extern "C" {

extern Agent7Root *D_089D5EA0;
int func_08867984(int key, int flags);

void func_08862CDC(int key, unsigned char on)
{
    int entry = func_08867984(key, 0);
    D_089D5EA0->manager->setEntry14(entry, on);
}

}
