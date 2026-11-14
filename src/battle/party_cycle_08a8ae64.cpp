typedef struct CycleUnit {
    char pad_000[0x600];
    CycleUnit *next;
} CycleUnit;

typedef struct CycleParty {
    char pad_000[0x130];
    CycleUnit *head;

    static CycleUnit *nextOf(CycleParty *party, CycleUnit *unit)
    {
        return unit == 0 ? party->head : unit->next;
    }
} CycleParty;

typedef struct PartyCycler {
    char pad_00[0x38];
    int index;
} PartyCycler;

extern "C" {

extern CycleParty *func_0881BAA8(void);

CycleUnit *func_08A8AE64(PartyCycler *self)
{
    CycleParty *party = func_0881BAA8();
    int count = 0;
    CycleUnit *walk = 0;
    while ((walk = CycleParty::nextOf(party, walk)) != 0) {
        count++;
    }
    if (count <= 0) {
        return 0;
    }
    self->index = (self->index + 1) % count;
    CycleUnit *unit = party->head;
    for (int i = 0; i < self->index; i++) {
        unit = unit->next;
    }
    return unit;
}

}
