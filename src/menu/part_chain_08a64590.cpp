struct A6Entry9 {
    short delta;
    void (*fn)(char *);
};

struct A6Table9 {
    char pad_00[8];
    A6Entry9 entry;
};

struct A6Chain9 {
    char pad_00[0xC];
    A6Table9 *field_0C;
};

extern "C" {
void func_08A64A58(void);
void func_08A64990(A6Chain9 *self);
void func_08A653CC(void);
void func_08A652F0(A6Chain9 *self);

void func_08A64590(A6Chain9 *self) {
    func_08A64A58();
    func_08A64990(self);
    A6Entry9 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}
}
