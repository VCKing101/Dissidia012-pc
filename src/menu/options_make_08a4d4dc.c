#include "menu/agent36_types.h"

/* Builds the options screen object: its task and a reset option state. */

typedef struct Object4D4DC {
    int field_00;
    char task[0x3C];          /* +0x04 */
    int field_40;
    char pad_44[0x4];
    int field_48;
} Object4D4DC;

void func_08864FF0(void *task, int a, int size, int b);

Object4D4DC *func_08A4D4DC(Object4D4DC *self)
{
    func_08864FF0(self->task, 1, 0x11180, 0x4C);
    D_09B6DCE8.field_11 = 0;
    D_09B6DCE8.field_12 = 0;
    D_09B6DCE8.field_00 = 0;
    D_09B6DCE8.field_10 = 1;
    D_09B6DCE8.field_04 = 0;
    D_09B6DCE8.field_13 = 0;
    D_09B6DCE8.field_14 = 0;
    D_09B6DCE8.field_15 = 0;
    self->field_00 = 0;
    self->field_48 = 0;
    self->field_40 = 0;
    return self;
}
