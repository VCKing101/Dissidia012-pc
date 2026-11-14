#include "main/agent30_types.h"

/* Hit area deleting destructor: unlinks the area from the global list. */
extern "C" {

extern HitAreaList D_089D4770;

void func_08992F04(HitArea *self);

}

struct HitAreaDtor0880ABF4 {
    static void destroy(HitArea *self, unsigned int flags)
    {
        D_089D4770.remove(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
};

extern "C" {

void func_0880ABF4(HitArea *self, unsigned int flags)
{
    if (self != 0) {
        HitAreaDtor0880ABF4::destroy(self, flags);
    }
}

}
