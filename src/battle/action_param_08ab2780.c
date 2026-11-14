#include "battle/agent23_types.h"

typedef struct ParamEntry {
    char pad_00[0x22];
    short param;
} ParamEntry;

extern int func_08AAE300(UnitParamOwner *self);
extern int func_08AAE84C(UnitParamOwner *self);
extern ParamEntry *func_08AACEE0(UnitParamTable *table, int key, int kind);
extern int func_08AAD148(UnitParamTable *table, int param);

/* Looks up the parameter named by the owner's current action entry; 0 when there is none. */
int func_08AB2780(UnitParamOwner *self)
{
    int key;
    UnitParamTable *table;
    int kind;
    ParamEntry *entry;
    if (self->active == 0) {
        return 0;
    }
    key = func_08AAE300(self);
    if (key < 0) {
        return 0;
    }
    table = func_08A80200(D_08A80218);
    func_08AAE300(self);
    kind = func_08AAE84C(self);
    entry = func_08AACEE0(table, key, kind);
    if (entry == 0) {
        return 0;
    }
    return func_08AAD148(table, entry->param);
}
