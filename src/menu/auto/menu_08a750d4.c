#include "menu/auto_region3.h"

/* Rows74E80: sets the second row's text. */
void func_08A750D4(Rows74E80 *self, const char *text)
{
    func_08A74CBC(&self->rows[1], text, 1, 4);
}
