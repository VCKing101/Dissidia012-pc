typedef struct ScaleTable {
    unsigned char active;
    char pad_01[0x3];
    float scales[10];
} ScaleTable;

extern int func_08A6217C(ScaleTable *self);

float func_08A62184(ScaleTable *self, unsigned int index)
{
    float *scales;

    if (!func_08A6217C(self)) {
        return 0.0f;
    }
    if (index >= 10) {
        return 0.0f;
    }
    scales = self->scales;
    return scales[index];
}
