typedef struct TripleSetup {
    unsigned char active;
    char pad_01[0x83];
    void *first;
    void *second;
    void *third;
} TripleSetup;

extern void func_08A981C0(TripleSetup *self);
extern int func_08A98A0C(TripleSetup *self);

int func_08A98210(TripleSetup *self, void *first, void *second, void *third)
{
    func_08A981C0(self);
    if (first == 0 || second == 0 || third == 0) {
        return -1;
    }
    if (func_08A98A0C(self) != 0) {
        return -2;
    }
    self->first = first;
    self->second = second;
    self->third = third;
    self->active = 1;
    return 0;
}
