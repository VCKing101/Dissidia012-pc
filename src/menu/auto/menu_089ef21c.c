#include "menu/auto_region1.h"

extern int func_088D6254(int value);

/* Screen0B6A0: func_088D6254 of rows[index].field_0D. */
unsigned short func_089EF21C(Screen0B6A0 *self, Row18 *rows, int index)
{
    unsigned short value;

    value = func_088D6254(rows[index].field_0D);
    return value;
}
