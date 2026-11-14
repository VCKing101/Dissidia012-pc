#include "menu/auto_region3.h"

/* Rows74E80: sets the first row's text. */
void func_08A750B4(Rows74E80 *self, const char *text)
{
    func_08A74CBC(&self->rows[0], text, 1, 3);
}
