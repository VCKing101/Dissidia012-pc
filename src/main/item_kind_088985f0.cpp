/* Kind of item idx, or -1 when idx is out of range. */
struct Item088985F0 {
    char pad_00[0x22];
    short kind;
    char pad_24[0x28];
};

struct Info088985F0 {
    char pad_00[0x14];
    int count;
};

struct Obj088985F0 {
    char pad_00[0x58];
    Info088985F0 *info;
    char pad_5C[0x14];
    union {
        Item088985F0 *items;
        void *data;
    };

    static bool valid(Obj088985F0 *o, int idx)
    {
        if (o->data != 0 && idx >= 0 && idx < o->info->count) {
            return true;
        }
        return false;
    }
};

extern "C" {

int func_088985F0(Obj088985F0 *o, int idx)
{
    if (Obj088985F0::valid(o, idx)) {
        return o->items[idx].kind;
    }
    return -1;
}

}
