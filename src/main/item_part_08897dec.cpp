/* Returns field 0x24 of item idx, or 0 when idx is out of range. */
struct Item08897DEC {
    char pad_00[0x24];
    char field_24[0xC];
    char field_30[0xC];
};

struct Info08897DEC {
    int count;
};

struct Obj08897DEC {
    char pad_00[0x58];
    Info08897DEC *info;
    char pad_5C[0x10];
    Item08897DEC *items;

    static bool valid(Obj08897DEC *o, int idx)
    {
        if (o->items != 0 && idx >= 0 && idx < o->info->count) {
            return true;
        }
        return false;
    }
};

extern "C" {

void *func_08897DEC(Obj08897DEC *o, int idx)
{
    if (Obj08897DEC::valid(o, idx)) {
        return o->items[idx].field_24;
    }
    return 0;
}

}
