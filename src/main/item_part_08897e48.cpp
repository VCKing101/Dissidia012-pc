/* Returns field 0x30 of item idx, or 0 when idx is out of range. */
struct Item08897E48 {
    char pad_00[0x24];
    char field_24[0xC];
    char field_30[0xC];
};

struct Info08897E48 {
    int count;
};

struct Obj08897E48 {
    char pad_00[0x58];
    Info08897E48 *info;
    char pad_5C[0x10];
    Item08897E48 *items;

    static bool valid(Obj08897E48 *o, int idx)
    {
        if (o->items != 0 && idx >= 0 && idx < o->info->count) {
            return true;
        }
        return false;
    }
};

extern "C" {

void *func_08897E48(Obj08897E48 *o, int idx)
{
    if (Obj08897E48::valid(o, idx)) {
        return o->items[idx].field_30;
    }
    return 0;
}

}
