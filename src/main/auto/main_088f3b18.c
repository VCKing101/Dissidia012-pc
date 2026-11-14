#include "main/auto_region4.h"

/* Clears the records 1..0x20 (all but 0x17) of the table at D_089D712C + 0xF58. */
void func_088F3B18(void)
{
    Record8CB164 *record;
    int i;

    i = 1;
    do {
        if (i != 0x17) {
            record = func_088CB164(D_089D712C->field_F58, i);
            record->field_0C = 0;
            record->field_06 = 0;
        }
        i = i + 1;
    } while (i < 0x21);
}
