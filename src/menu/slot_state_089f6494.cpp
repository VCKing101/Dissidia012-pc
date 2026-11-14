/* 0 for a type-0x3D part, 2 (after a notice) when its flag 0x20 is set, else 1. */
struct StatePart {
    char pad_00[0xC];
    short field_0C;
    char pad_0E[0x20 - 0xE];
    unsigned short field_20;

    static bool flagged(StatePart *part) { return (part->field_20 & 0x20) != 0; }
};

/* Ten part pointers per slot from +0x51C. */
struct StateTable {
    char pad_000[0x51C];
    StatePart *field_51C[1];

    static StatePart *part(StateTable *table, int index) { return table->field_51C[index]; }
};

struct StateOwner {
    char pad_000[0x14C];
    StateTable *field_14C;
};

extern "C" int func_089F2FF0(StateOwner *self);
extern "C" void func_089F5990(StateOwner *self, int flag);

extern "C" {
int func_089F6494(StateOwner *self)
{
    int result = 0;
    int slot = func_089F2FF0(self);
    if (StateTable::part(self->field_14C, slot * 10)->field_0C != 0x3D) {
        if (StatePart::flagged(StateTable::part(self->field_14C, slot * 10))) {
            func_089F5990(self, 0x80);
            result = 2;
        } else {
            result = 1;
        }
    }
    return result;
}
}
