struct A6EntryA {
    short delta;
    void (*fn)(char *);
};

struct A6TableA {
    char pad_00[8];
    A6EntryA entry;
};

struct A6ChainA {
    char pad_00[0xC];
    A6TableA *field_0C;
};

extern "C" {
void func_08A653CC(void);
void func_08A652F0(A6ChainA *self);

void func_08A64CE4(A6ChainA *self) {
    func_08A653CC();
    func_08A652F0(self);
    A6EntryA *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}
}
