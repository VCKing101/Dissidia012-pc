#include "battle/agent23_types.h"

struct ActionRates {
    float field_00;
    float field_04;
    float rate_08;
    float rate_0C;
};

extern "C" {

extern int func_08AAE300(UnitParamOwner *self);
extern int func_08AAE84C(UnitParamOwner *self);
extern ActionRates *func_08AAC874(UnitParamTable *table, int key, int kind);

/* 1 when a random roll falls below the owner's action rate at 0x08. */
unsigned char func_08AAFA24(UnitParamOwner *self)
{
    UnitParamTable *table;
    int key;
    int kind;
    ActionRates *rates;
    if (self->active == 0) {
        return 0;
    }
    table = func_08A80200(D_08A80218);
    key = func_08AAE300(self);
    kind = func_08AAE84C(self);
    rates = func_08AAC874(table, key, kind);
    if (rates == 0) {
        return 0;
    }
    return UnitRoll::below(rates->rate_08);
}

}
