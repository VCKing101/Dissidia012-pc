typedef struct UnitBinder {
    unsigned char active;
    char pad_01[0x3];
    int id;
} UnitBinder;

extern void *func_08AB32B8(int id);
extern int func_08A80F28(UnitBinder *self, void *unit);

int func_08A80EBC(UnitBinder *self)
{
    void *unit;

    if (self->active == 0) {
        return -1;
    }
    unit = func_08AB32B8(self->id);
    if (unit == 0) {
        return -2;
    }
    if (func_08A80F28(self, unit) != 0) {
        return -3;
    }
    return 0;
}
