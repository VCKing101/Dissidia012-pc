/* Returns block 0x34 of item idx, or 0 when idx is out of range. */
struct Item08898934 {
    char pad_00[0x22];
    short kind;
    short field_24;
    unsigned char flag;
    char pad_27[1];
    char field_28[0xC];
    char field_34[0xC];
    char field_40[0xC];
};

struct Info08898934 {
    char pad_00[0x14];
    int count;
};

struct Obj08898934 {
    char pad_00[0x58];
    Info08898934 *info;
    char pad_5C[0x14];
    Item08898934 *items;

    static bool valid(Obj08898934 *o, int idx)
    {
        if (o->items != 0 && idx >= 0 && idx < o->info->count) {
            return true;
        }
        return false;
    }
};

extern "C" {

void * func_08898934(Obj08898934 *o, int idx)
{
    if (Obj08898934::valid(o, idx)) {
        return o->items[idx].field_34;
    }
    return 0;
}

}
