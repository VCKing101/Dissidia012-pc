#include "menu/agent36_types.h"

typedef struct Work4E8B8 {
    char pad_00[0x4];
    int state;                /* +0x04 */
    char pad_08[0x8];
    RootOptions36 *source;    /* +0x10 */
    RootOptions36 options;    /* +0x14 */
} Work4E8B8;

typedef struct Screen4E8B8 {
    char pad_00[0x40];
    int field_40;             /* +0x40 */
    char pad_44[0x18];
    Work4E8B8 *work;          /* +0x5C */
} Screen4E8B8;

int func_08935490(const void *a, const void *b, int size);
void func_08A4E960(Screen4E8B8 *self, int page);
void func_08A4D07C(Screen4E8B8 *self, int a, float b);

void func_08A4E8B8(Screen4E8B8 *self, int a, int page, int c)
{
    if (func_08935490(&self->work->options, self->work->source, sizeof(RootOptions36)) != 0) {
        D_09B6DCE8.field_13 = 1;
    }
    func_08A4E960(self, page);
    D_09B6DCE8.field_04 = a;
    D_09B6DCE8.field_08 = c;
    self->field_40 = 3;
    func_08A4D07C(self, 1, 0.0f);
    self->work->state = 3;
}
