/* Image library: reorder a 256-entry CLUT between the PS2 (CSM1) and PSP layouts. */
#include "main/sdk_gcc1_types.h"

void func_0899E900(unsigned int *clut, int format, int count)
{
    int i, j;

    if (count != 256)
        return;
    if (format == 1) {
        for (j = 0; j < 8; j++) {
            for (i = 0; i < 4; i++) {
                unsigned int t = clut[4 + i];
                clut[4 + i] = clut[8 + i];
                clut[8 + i] = t;
            }
            clut += 16;
        }
    } else if (format == 3) {
        for (j = 0; j < 8; j++) {
            for (i = 0; i < 8; i++) {
                unsigned int t = clut[8 + i];
                clut[8 + i] = clut[16 + i];
                clut[16 + i] = t;
            }
            clut += 32;
        }
    }
}
