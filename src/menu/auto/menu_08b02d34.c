#include "menu/auto_region4.h"

extern const char D_08B04550[];

/* Formats value with the format at D_08B04550 ("-" for -1 when flag is
 * set) and hands the text to func_08B02C20. */
void func_08B02D34(void *self, int arg, int value, int flag)
{
    int i;
    char text[16];

    i = 0;
    do {
        text[i] = 0;
        i = i + 1;
    } while (i < 0x10);
    if (flag == 0) {
        func_0893476C(text, (int)D_08B04550, value);
    }
    else if (value == -1) {
        text[0] = '-';
        text[1] = 0;
    }
    else {
        func_0893476C(text, (int)D_08B04550, value);
    }
    func_08B02C20(self, arg, text);
}
