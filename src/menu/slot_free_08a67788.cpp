struct FreeRecord {
    char pad_00[0x64];
    unsigned short field_64;

    static bool isFree(FreeRecord *record)
    {
        if (record->field_64 == 10) {
            return true;
        }
        return false;
    }
};

struct FreeSlots {
    int field_00;
    FreeRecord *field_04;
};

struct FreeOwner {
    char pad_00[0xC4];
    FreeSlots *field_C4;
};

extern "C" {
int func_08A67788(FreeOwner *self)
{
    int result = -1;
    for (int i = 0; i < 10; i++) {
        if (FreeRecord::isFree(&self->field_C4->field_04[i]) == true) {
            result = i;
            break;
        }
    }
    return result;
}
}
