typedef struct Setup08A084C8 {
    unsigned char active;
    char pad_01[0x7];
    char link_08[0x4];
    char pad_0C[0x1C];
    unsigned int field_28;
    unsigned int field_2C;
    void *field_30;
} Setup08A084C8;

extern void func_08A08480(Setup08A084C8 *self);
extern int func_089F7D74(void *link, void *arg0, void *side);

/* Validates the arguments and records them; -1 when one is out of range. */
int func_08A084C8(Setup08A084C8 *self, unsigned int side, unsigned int slot, void *unit, void *source, void *target)
{
    func_08A08480(self);
    if (side >= 2 || slot >= 2 || unit == 0 || source == 0 || target == 0) {
        return -1;
    }
    func_089F7D74(self->link_08, source, target);
    self->field_28 = side;
    self->field_2C = slot;
    self->field_30 = unit;
    self->active = 1;
    return 0;
}
