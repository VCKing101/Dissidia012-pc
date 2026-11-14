#include "menu/auto_region1.h"

/* Copies the matrix and colour of source's entry index onto text. */
void func_089ED134(void *self, void *text, void *source, int index)
{
    float matrix[16];
    unsigned int color;

    if (text != 0) {
        func_088DA0A8(source, index, matrix, &color);
        func_0887B778(text, matrix);
        func_0887B820(text, color);
    }
}
