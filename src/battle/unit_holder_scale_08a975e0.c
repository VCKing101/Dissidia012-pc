typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x27];
    float scale;
} UnitHolder;

int func_08A975E0(UnitHolder *self, float scale)
{
    if (self->active == 0) {
        return -1;
    }
    self->scale = scale;
    return 0;
}
