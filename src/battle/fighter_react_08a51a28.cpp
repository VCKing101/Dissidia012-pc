#include "battle/agent11_types.h"

extern "C" {

extern int func_08838120(BattleFighter *fighter);
extern int func_088380F8(BattleFighter *fighter);
extern void func_08834934(BattleFighter *fighter, int motion, int next);
extern void func_08834948(BattleFighter *fighter);

void func_08A51A28(int unused, BattleFighter *self)
{
    if (func_08838120(self) != 0 || func_088380F8(self) != 0) {
        self->slot_0C8();
    } else {
        func_08834934(self, 0x65, 0x66);
        func_08834948(self);
    }
}

}
