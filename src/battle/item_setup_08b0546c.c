typedef struct ItemSetup {
    unsigned char active;
    char pad_01[0x13];
    unsigned int side;
    void *first;
    void *second;
} ItemSetup;

extern void func_08B0532C(ItemSetup *self);

int func_08B0546C(ItemSetup *self, unsigned int side, void *first, void *second)
{
    func_08B0532C(self);
    if (side >= 2 || first == 0 || second == 0) {
        return -1;
    }
    self->side = side;
    self->first = first;
    self->second = second;
    self->active = 1;
    return 0;
}
