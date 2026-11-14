#include "menu/auto_region4.h"

/* Moves along the larger of x and z of vector (+1 or -1). */
void func_08B4A5CC(void *self, int arg, float *vector, unsigned char flag)
{
    float z;
    float x;

    x = vector[0];
    z = vector[2];
    if (x * x < z * z) {
        if (0.0f < z) {
            func_08B4A6A8(self, arg, 1, flag);
        }
        else {
            func_08B4A6A8(self, arg, -1, flag);
        }
    }
    else if (0.0f < x) {
        func_08B4A7D8(self, arg, 1, flag);
    }
    else {
        func_08B4A7D8(self, arg, -1, flag);
    }
}
