struct FreeSlot {
    signed char field_00;
    char pad_01[0xF];

    static bool isFree(FreeSlot *slot)
    {
        if (slot->field_00 == -1) {
            return true;
        }
        return false;
    }
};

struct FreeSlots {
    FreeSlot *field_00;
};

struct FreeOwner {
    char pad_00[0xC4];
    FreeSlots *field_C4;
};

extern "C" {
int func_08A6772C(FreeOwner *self)
{
    int result = -1;
    for (int i = 2; i < 0x16; i++) {
        if (FreeSlot::isFree(&self->field_C4->field_00[i]) == true) {
            result = i;
            break;
        }
    }
    return result;
}
}
