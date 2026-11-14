#include "menu/agent29_types.h"

/* ScreenE75C4: starts a fade from white, switches to the sound of the
   current page when it differs from the one playing, and moves to state
   0xE. Retail keeps the sound entry in the frame across the stop call. */
extern "C" {
extern char D_089E0AA4[];
void func_088853CC(void *object, char value);
void func_089E30D8(ScreenE75C4Ex *self);

void func_089E1704(ScreenE75C4Ex *self)
{
    FadeColor29 color;

    func_08873D34((float)func_088D4C5C(), 3, D_089E0AA4, 0, 1, 2);
    color.r = 255.0f;
    color.g = 255.0f;
    color.b = 255.0f;
    color.a = 255.0f;
    func_08873DB0(&color, 2);
    Page71C0 *entry = &D_089E71C0[WORK_INT(self->work, 0xB1C)];
    if (entry->sound != self->sound.id) {
        Kept<Page71C0 *> saved(entry);
        func_088D54DC(&self->sound);
        self->sound.id = saved.value->sound;
        func_088D55AC(&self->sound, self->heap, self->field_134);
    }
    int none = 0;
    func_088853CC(D_089D69E8, none);
    func_089E30D8(self);
    func_089E2CD0((ScreenE75C4 *)self);
    self->work->state = 0xE;
}
}
