#include "menu/agent36_types.h"

typedef struct Work4E660 {
    char pad_0000[0xC];
    void *messages;           /* +0x000C */
    char pad_0010[0x6B14];
    char *names;              /* +0x6B24: name (0x10) then text */
    const char *name;         /* +0x6B28 */
    const char *text;         /* +0x6B2C */
    char pad_6B30[0x4C4];
    void *list;               /* +0x6FF4 */
} Work4E660;

typedef struct Screen4E660 {
    char pad_00[0x5C];
    Work4E660 *work;          /* +0x5C */
} Screen4E660;

void func_08935524(void *dst, const void *src, int size);   /* memcpy */
void func_08A5DFE8(void *list, char *name, char *text, int a, int b, void *messages);

void func_08A4E660(Screen4E660 *self, const char *name, int a, int b, const char *text)
{
    if (D_09B6DCE8.field_15 == 0) {
        func_08935524(self->work->names, name, 0x10);
        func_08935524(self->work->names + 0x10, text, 0x66);
        D_09B6DCE8.field_15 = 1;
    }
    self->work->name = name;
    self->work->text = text;
    func_08A5DFE8(self->work->list, self->work->names, self->work->names + 0x10, a, b, self->work->messages);
}
