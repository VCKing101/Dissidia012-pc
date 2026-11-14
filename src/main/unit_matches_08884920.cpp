/* 1 when the unit's id is the entry's first id, or its second id without flag 0x20. */
struct Pair08884920 {
    unsigned short first;
    short field_02;
    unsigned short second;
    short field_06;
    int field_08;
};

struct Unit08884920 {
    char pad_00[0xC];
    short id;
    char pad_0E[0x12];
    unsigned short flags;

    static bool has(Unit08884920 *u, unsigned int mask) { return (u->flags & mask) != 0; }
};

struct Obj08884920 {
    char pad_00[0xC];
    Unit08884920 *unit;
    char pad_10[8];
    int index;
};

extern "C" {

extern Pair08884920 D_089C9F70[];

int func_08884920(Obj08884920 *o)
{
    int result = 0;
    Unit08884920 *u = o->unit;
    if (u != 0) {
        Pair08884920 *p = &D_089C9F70[o->index];
        if (u->id == p->first) {
            result = 1;
        } else if (u->id == p->second && !Unit08884920::has(u, 0x20)) {
            result = 1;
        }
    }
    return result;
}

}
