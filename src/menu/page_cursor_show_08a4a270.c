#include "menu/agent36_types.h"

typedef struct Work4A270 {
    char pad_000[0x40C];
    char layout[0x10];        /* +0x40C */
    Model36 *models[7];       /* +0x41C */
    char pad_438[0x458];
    void *texts[7];           /* +0x890 */
    void *sprite;             /* +0x8AC */
    char pad_8B0[0x1D8];
    int index;                /* +0xA88 */
} Work4A270;

typedef struct Screen4A270 {
    char pad_000[0x14C];
    Work4A270 *work;          /* +0x14C */
} Screen4A270;

extern ScriptCmd36 D_08A4BED4[];
void func_088D40BC(void *layout, void *texts, unsigned short id, unsigned short x, unsigned short y, int kind);

void func_08A4A270(Screen4A270 *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        func_088D9F5C(self->work->models[i], 3, 1);
    }
    func_088D428C(self->work->layout, self->work->texts, D_08A4BED4, 1);
    if (self->work->sprite != 0) {
        func_0887BA5C(self->work->sprite);
        self->work->sprite = 0;
    }
    self->work->sprite = func_0887E534(self->work->texts[self->work->index]);
    {
        Model36 *model = self->work->models[self->work->index];

        model->field_34 = 0;
        model->field_30 = 0;
        model->field_38 = 0;
    }
    func_088D9F5C(self->work->models[self->work->index], 2, 1);
    func_088D40BC(self->work->layout, self->work->texts, 7, 9, self->work->index + 4, 1);
    func_088D40BC(self->work->layout, self->work->texts, self->work->index, 9, self->work->index + 4, 2);
}
