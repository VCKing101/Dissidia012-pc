#include "main/auto_region2.h"

/* func_0886A9B0 for each of the eight slots of a model part; returns 1. */
int func_0886AA98(ModelObj *self, int part)
{
    short value;
    int i;

    value = self->data->parts[part].field_50;
    i = 0;
    do {
        func_0886A9B0(self, part, value, i);
        i = i + 1;
    } while (i < 8);
    return 1;
}
