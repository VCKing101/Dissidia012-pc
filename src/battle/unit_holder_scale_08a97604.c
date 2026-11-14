typedef struct UnitHolder {
    unsigned char active;
    char pad_01[0x3];
    void *unit;
    char pad_08[0x20];
    float scale;
} UnitHolder;

extern int func_08A84124(void *unit);
extern int func_08A9771C(UnitHolder *self);

float func_08A97604(UnitHolder *self)
{
    if (self->active == 0) {
        return 0.0f;
    }
    if (func_08A84124(self->unit) == 1) {
        return 0.0f;
    }
    if (func_08A9771C(self) == 1) {
        return 0.0f;
    }
    return self->scale;
}
