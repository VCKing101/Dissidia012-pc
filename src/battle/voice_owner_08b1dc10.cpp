#include "battle/agent27_types.h"

extern "C" {

struct VoiceOwnerSlot {
    int field_00;
    int state;          /* 0x04 */
    int field_08;
    Actor544 *owner;    /* 0x0C */
    char pad10[8];
    unsigned char indirect; /* 0x18 */
};

int func_08B1DBC8(VoiceOwnerSlot *slot);
int func_08B1DBD0(VoiceOwnerSlot *slot);

int func_08B1DC10(VoiceOwnerSlot *slot)
{
    if (func_08B1DBC8(slot) == 0) {
        return 0;
    }
    if (func_08B1DBD0(slot) == 0) {
        return 0;
    }
    if (slot->indirect == 1) {
        return slot->owner->slot_130();
    }
    return (int)slot->owner;
}

}
