typedef struct Setup08A20210 {
    unsigned char active;
    char pad_01[0x3];
    char link_04[0x4];
    char pad_08[0x20];
    unsigned int field_28;
    unsigned int field_2C;
    void *field_30;
} Setup08A20210;

extern void func_08A201C8(Setup08A20210 *self);
extern int func_089F7D74(void *link, void *source, void *target);

/* Validates the arguments and records them; -1 when one is out of range, -2 when the link fails. */
int func_08A20210(Setup08A20210 *self, unsigned int side, unsigned int slot, void *source, void *target, void *unit)
{
    func_08A201C8(self);
    if (side >= 2 || slot >= 2 || source == 0 || target == 0 || unit == 0) {
        return -1;
    }
    if (func_089F7D74(self->link_04, source, target) != 0) {
        return -2;
    }
    self->field_28 = side;
    self->field_2C = slot;
    self->field_30 = unit;
    self->active = 1;
    return 0;
}
