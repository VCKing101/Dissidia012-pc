typedef struct SlotSetup {
    unsigned char active;
    char pad_01[0x3];
    int count;
    char pad_08[0x8];
    unsigned int side;
    unsigned int slot;
    void *unit;
    unsigned char mode;
} SlotSetup;

extern void func_08A1681C(SlotSetup *self);

/* Records side, slot, unit and mode and clears the count; -1 for bad arguments. */
int func_08A16874(SlotSetup *self, unsigned int side, unsigned int slot, void *unit, unsigned char mode)
{
    func_08A1681C(self);
    if (side >= 2 || slot >= 2 || unit == 0) {
        return -1;
    }
    self->side = side;
    self->slot = slot;
    self->unit = unit;
    self->mode = mode;
    self->active = 1;
    self->count = 0;
    return 0;
}
