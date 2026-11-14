struct A6Entry5 {
    short delta;
    void (*fn)(char *);
};

struct A6Table5 {
    char pad_00[0x28];
    A6Entry5 entry;
};

struct A6Owner5 {
    char pad_0000[0x1984];
    char field_1984[0x24];
    A6Table5 *field_19A8;
};

extern "C" {
void func_08A51F3C(void);

void func_08A5A0D8(A6Owner5 *self) {
    func_08A51F3C();
    A6Entry5 *e = &self->field_19A8->entry;
    char *sub = self->field_1984;
    e->fn(sub + e->delta);
}
}
