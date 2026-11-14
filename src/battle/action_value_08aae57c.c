#include "battle/agent23_types.h"

typedef struct ActionUnit {
    char pad_00[0x30];
    int state;
} ActionUnit;

typedef struct ActionOwner {
    unsigned char active;
    char pad_01[0x3];
    ActionUnit *unit;
} ActionOwner;

typedef struct ActionEntry {
    char pad_00[0x6];
    short value;
} ActionEntry;

extern int func_08AAE300(ActionOwner *self);
extern int func_08AAE84C(ActionOwner *self);
extern ActionEntry *func_08AAC3EC(UnitParamTable *table, int key, int kind);

/* Value of the owner's current action entry; 2 while the unit is in state 2, -1 when inactive or missing. */
int func_08AAE57C(ActionOwner *self)
{
    UnitParamTable *table;
    int key;
    int kind;
    ActionEntry *entry;
    if (self->active == 0) {
        return -1;
    }
    if (self->unit->state == 2) {
        return 2;
    }
    table = func_08A80200(D_08A80218);
    key = func_08AAE300(self);
    kind = func_08AAE84C(self);
    entry = func_08AAC3EC(table, key, kind);
    if (entry == 0) {
        return -1;
    }
    return entry->value;
}
