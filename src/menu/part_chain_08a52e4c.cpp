struct A6Entry {
    short delta;
    void (*fn)(char *);
};

struct A6Table {
    char pad_00[8];
    A6Entry entry;
};

struct A6Chain {
    char pad_000[0x84];
    A6Table *field_84;
};

extern "C" {
void func_08A5318C(void);
void func_08A530D0(A6Chain *self);

void func_08A52E4C(A6Chain *self) {
    func_08A5318C();
    func_08A530D0(self);
    A6Entry *e = &self->field_84->entry;
    e->fn((char *)self + e->delta);
}
}
