typedef struct PairSetupB {
    unsigned char active;
    char pad_01[0x3];
    void *first;
    void *second;
    char field_0C[0x4];
} PairSetupB;

extern void func_08A9A5EC(PairSetupB *self);
extern int func_08A7E7FC(void *sub);

int func_08A9A61C(PairSetupB *self, void *first, void *second)
{
    func_08A9A5EC(self);
    if (first == 0 || second == 0) {
        return -1;
    }
    if (func_08A7E7FC(self->field_0C) != 0) {
        return -2;
    }
    self->first = first;
    self->second = second;
    self->active = 1;
    return 0;
}
