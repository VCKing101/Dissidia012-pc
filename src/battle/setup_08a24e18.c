typedef struct Setup08A24E18 {
    unsigned char active;
    char pad_01[0x7];
    char link_08[0x4];
    char pad_0C[0x6C];
    unsigned int field_78;
    void *field_7C;
    void *field_80;
} Setup08A24E18;

extern void func_08A24D40(Setup08A24E18 *self);
extern int func_089F7D74(void *link, void *arg0, void *side);

/* Validates the arguments and records them; -1 when one is out of range. */
int func_08A24E18(Setup08A24E18 *self, unsigned int side, void *slot, void *unit, void *source, void *target)
{
    func_08A24D40(self);
    if (side >= 2 || slot == 0 || unit == 0 || source == 0 || target == 0) {
        return -1;
    }
    func_089F7D74(self->link_08, source, target);
    self->field_78 = side;
    self->field_7C = slot;
    self->field_80 = unit;
    self->active = 1;
    return 0;
}
