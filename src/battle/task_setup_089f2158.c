typedef struct SetupTask {
    unsigned char active;
    char pad_01[0x7];
    char part_08[0x20];
    char part_28[0x128];
    unsigned int side;
    unsigned int slot;
    int unit;
} SetupTask;

extern void func_089F2108(SetupTask *self);
extern void func_089F7D74(void *part, void *source, void *target);

/* Sets the task up for side, slot and unit with source and target; -1 for bad arguments. */
int func_089F2158(SetupTask *self, unsigned int side, unsigned int slot, int unit, void *source, void *target)
{
    func_089F2108(self);
    if (side >= 2 || slot >= 2 || unit == 0 || source == 0 || target == 0) {
        return -1;
    }
    func_089F7D74(self->part_08, source, target);
    func_089F7D74(self->part_28, source, target);
    self->side = side;
    self->slot = slot;
    self->unit = unit;
    self->active = 1;
    return 0;
}
