typedef struct LinkSetup {
    unsigned char active;
    char pad_01[0x3];
    unsigned int side;
    unsigned int slot;
    void *source;
    char link[0x4];
} LinkSetup;

extern void func_089F5F44(LinkSetup *self);
extern int func_089F7D74(void *link, void *source, void *target);

/* Links source to target and records side, slot and source; -1 bad arguments, -2 link failed. */
int func_089F5F74(LinkSetup *self, unsigned int side, unsigned int slot, void *source, void *target)
{
    func_089F5F44(self);
    if (side >= 2 || slot >= 3 || source == 0 || target == 0) {
        return -1;
    }
    if (func_089F7D74(self->link, source, target) != 0) {
        return -2;
    }
    self->side = side;
    self->slot = slot;
    self->source = source;
    self->active = 1;
    return 0;
}
