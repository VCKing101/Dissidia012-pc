#include "battle/agent23_types.h"

extern "C" {

extern int func_08B0FB28(HandleOwner *self);

/* True when the linked object was switched off. */
bool func_08B0F9C4(HandleOwner *self)
{
    if (func_08B0FB28(self) == 1) {
        return true;
    }
    return false;
}

}
