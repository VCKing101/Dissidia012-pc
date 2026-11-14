#include "battle/agent23_types.h"

extern "C" {

extern void *func_08B11718(int handle);
extern int func_08A810C0(void *object);
extern void func_08A80B90(void *object, int on);

/* Switches the linked object off when it is in state 1; true when it did. */
bool func_08B0FB28(HandleOwner *self)
{
    void *object = func_08B11718(self->handle);
    if (object != 0 && func_08A810C0(object) == 1) {
        func_08A80B90(object, 0);
        return true;
    }
    return false;
}

}
