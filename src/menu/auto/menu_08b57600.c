#include "menu/auto_region4.h"

/* Copies the matrix and colour of source's entry index onto text. */
void func_08B57600(void *self, void *text, void *source, int index)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(source, index, matrix, &color);
    func_0887B778(text, matrix);
    func_0887B820(text, color);
}
