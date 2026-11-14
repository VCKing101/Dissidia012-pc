#include "menu/auto_region3.h"

extern const char D_08A75B50[];

/* Rows74E80: prints a value into the third row. */
void func_08A750F8(Rows74E80 *self, int value)
{
    char text[16];

    func_089347E8(text, D_08A75B50, value);
    func_08A74CBC(&self->rows[2], text, 1, 4);
}
