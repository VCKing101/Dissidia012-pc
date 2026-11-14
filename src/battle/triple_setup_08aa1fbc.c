typedef struct TripleSetupB {
    unsigned char active;
    char pad_01[0x3];
    void *first;
    void *second;
    void *third;
    char pad_10[0x4];
    char field_14[0x4];
} TripleSetupB;

extern void func_08AA1F8C(TripleSetupB *self);
extern int func_08A7E7FC(void *sub);

int func_08AA1FBC(TripleSetupB *self, void *first, void *second, void *third)
{
    func_08AA1F8C(self);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    if (func_08A7E7FC(self->field_14) != 0) {
        return -2;
    }
    self->first = first;
    self->second = second;
    self->third = third;
    self->active = 1;
    return 0;
}
