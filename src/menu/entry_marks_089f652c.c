#include "menu/agent20_types.h"

/* Sets the help text, then shows mark 0x2E on every entry flagged 0x40000. */
typedef struct MarkState {
    char pad_000[0x88];
    char field_88[0x400];
    char field_488[4];
    char pad_48C[0x224];
    int field_6B0[9];
    char pad_6D4[0x205D];
    signed char field_2731;
    char pad_2732[0x29A];
    A20Entry field_29CC[9];
} MarkState;

typedef struct MarkOwner {
    char pad_000[0x14C];
    MarkState *field_14C;
} MarkOwner;

extern void func_088D3B88(char *text, char *work, unsigned short *message, int start, int width);
extern void func_088D9F5C(int label, int value, int redraw);
extern unsigned short D_08A094B6[];

void func_089F652C(MarkOwner *self)
{
    int i;
    func_088D3B88(self->field_14C->field_88, self->field_14C->field_488, D_08A094B6, 0, 0x180);
    for (i = 0; i < self->field_14C->field_2731; i++) {
        if ((self->field_14C->field_29CC[i].field_04 & 0x40000) != 0) {
            func_088D9F5C(self->field_14C->field_6B0[i], 0x2E, 1);
        }
    }
}
