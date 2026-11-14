#include "battle/agent23_types.h"

struct ActionSpan {
    short start;
    short count;
};

struct ActionGroup {
    char pad_00[0x4];
    ActionSpan span;

    static short countOf(ActionSpan *span) { return ((short *)span)[1]; }
};

extern "C" {

extern int func_08AAE300(UnitParamOwner *self);
extern int func_08AAE84C(UnitParamOwner *self);
extern ActionGroup *func_08AAC584(UnitParamTable *table, unsigned int slot, int key, int kind);

/* Number of entries in the owner's action group for slot, 0 when there is none. */
int func_08AAEDFC(UnitParamOwner *self, unsigned int slot)
{
    if (self->active == 0) {
        return 0;
    }
    if (slot >= 3) {
        return 0;
    }
    UnitParamTable *table = func_08A80200(D_08A80218);
    int key = func_08AAE300(self);
    int kind = func_08AAE84C(self);
    ActionGroup *group = func_08AAC584(table, slot, key, kind);
    if (group == 0) {
        return 0;
    }
    return ActionGroup::countOf(&group->span);
}

}
