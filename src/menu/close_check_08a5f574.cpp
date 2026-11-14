/* Once the icon's flag 0x20 is set, runs virtual steps 9 and 8 and moves to state 5. */
#include "menu/agent16_types.h"

struct CloseIcon {
    char pad_00[0x20];
    unsigned short field_20;

    static bool flagged(CloseIcon *icon) { return (icon->field_20 & 0x20) != 0; }
};

class CloseCheckPart : public A16Part24 {
public:
    int field_28;
    char pad_02C[0x3C - 0x2C];
    CloseIcon *field_3C;
};

extern "C" {
void func_08A5F574(CloseCheckPart *self)
{
    if (!CloseIcon::flagged(self->field_3C)) {
        return;
    }
    self->v9();
    self->v8();
    self->field_18 = 0;
    self->field_28 = 5;
}
}
