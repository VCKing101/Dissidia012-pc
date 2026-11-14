typedef struct PairBind232F0 {
    unsigned char active;
    char pad_001[0xEF];
    void *field_F0;
    void *field_F4;
} PairBind232F0;

extern void func_08A232D4(PairBind232F0 *self);

int func_08A232F0(PairBind232F0 *self, void *first, void *second)
{
    func_08A232D4(self);
    if (!first || !second) {
        return -1;
    }
    self->field_F0 = first;
    self->field_F4 = second;
    self->active = 1;
    return 0;
}
