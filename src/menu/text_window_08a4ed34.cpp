#include "menu/agent36_types.h"

/* Opens the text window (text 0x1A of the page; a new text and argument
 * when text is not -1). */

struct Work4ED34 {
    char pad_0000[0xC];
    void *messages;           /* +0x000C */
    char pad_0010[0x6FB0];
    int field_6FC0;
    int text;                 /* +0x6FC4 */
    int arg;                  /* +0x6FC8 */
};

struct Screen4ED34 {
    char pad_00[0x3C];
    void *heap;               /* +0x3C */
    char pad_40[0x1C];
    Work4ED34 *work;          /* +0x5C */
};

extern "C" {
extern void *D_089D7F90;
extern char D_08A4FF1E[];
void func_088FD928(void *object, void *heap, int text, int b, void *name, int c, void *messages, int d,
                   int arg, int f, int g, int h);
void func_08A4EDC0(Screen4ED34 *self);
}


extern "C" void func_08A4ED34(Screen4ED34 *self, int text, int arg)
{
    if (text != -1) {
        self->work->text = text;
        self->work->arg = arg;
    }
    Kept<Screen4ED34 *> saved(self);
    func_08A4EDC0(self);
    self = saved.value;
    func_088FD928(D_089D7F90, self->heap, self->work->text, 0x1A, D_08A4FF1E, -1, self->work->messages, 0xD,
                  self->work->arg, 0, 0, self->work->field_6FC0);
}
