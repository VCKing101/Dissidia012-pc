struct A6EntryB {
    short delta;
    void (*fn)(char *);
};

struct A6TableB {
    char pad_00[8];
    A6EntryB entry;
};

struct A6ChainB {
    char pad_00[0xC];
    A6TableB *field_0C;
};

extern "C" {
void func_08A65ED8(void);
void func_08A65CCC(A6ChainB *self);

void func_08A655A4(A6ChainB *self) {
    func_08A65ED8();
    func_08A65CCC(self);
    A6EntryB *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}
}
