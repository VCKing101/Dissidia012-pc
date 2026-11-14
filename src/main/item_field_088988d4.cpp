/* Returns block 0x28 of item idx, or 0 when idx is out of range. */
struct Item088988D4 {
    char pad_00[0x22];
    short kind;
    short field_24;
    unsigned char flag;
    char pad_27[1];
    char field_28[0xC];
    char field_34[0xC];
    char field_40[0xC];
};

struct Info088988D4 {
    char pad_00[0x14];
    int count;
};

struct Obj088988D4 {
    char pad_00[0x58];
    Info088988D4 *info;
    char pad_5C[0x14];
    Item088988D4 *items;

    static bool valid(Obj088988D4 *o, int idx)
    {
        if (o->items != 0 && idx >= 0 && idx < o->info->count) {
            return true;
        }
        return false;
    }
};

extern "C" {

void * func_088988D4(Obj088988D4 *o, int idx)
{
    if (Obj088988D4::valid(o, idx)) {
        return o->items[idx].field_28;
    }
    return 0;
}

}
