#include "main/auto_region4.h"

/* field_1D8 / 90000, or -1 without the object. */
float func_088EE688(void)
{
    if (D_089D7F00 == 0) {
        return -1.0f;
    } else {
        return D_089D7F00->field_1D8 / 90000.0f;
    }
}
