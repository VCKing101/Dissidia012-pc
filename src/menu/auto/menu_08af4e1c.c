#include "menu/auto_region3.h"

/* ScreenB01CF8: builds a buffer, hands func_08AF9720's text to func_088CBCB4 and refreshes. */
void func_08AF4E1C(ScreenB01CF8 *self)
{
    char *text;
    char buffer[0x778];

    func_088CB9E8(buffer);
    text = func_08AF9720(buffer, self->work->field_00);
    func_088CBCB4(text);
    func_08AF3BE8(self);
}
