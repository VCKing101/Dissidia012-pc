/* Runs virtual step 10 of all 24 items, then the part's steps 9 and 8. */
#include "menu/agent16_types.h"

class HideItem {
public:
    char pad_00[0xC];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
};

class HidePart : public A16Part24 {
public:
    int field_28;
    char pad_02C[0xC8 - 0x2C];
    HideItem *field_C8[24];
};

extern "C" {
void func_08A665AC(HidePart *self)
{
    int i;
    for (i = 0; i < 24; i++) {
        self->field_C8[i]->v10();
    }
    self->v9();
    self->v8();
    self->field_28 = 0;
}
}
