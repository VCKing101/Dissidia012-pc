#include "battle/agent15_types.h"

extern void func_08ABE3F8(void *member);
extern int func_08AC22BC(void *member);

int func_08ABB2BC(ReadyOwner *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    func_08ABE3F8(self->member_DC);
    func_08AC22BC(self->member_16C);
    return 0;
}
