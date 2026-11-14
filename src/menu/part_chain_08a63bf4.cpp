struct A6Entry8 {
    short delta;
    void (*fn)(char *);
};

struct A6Table8 {
    char pad_00[8];
    A6Entry8 entry;
};

struct A6Chain8 {
    char pad_00[0xC];
    A6Table8 *field_0C;
};

extern "C" {
void func_08A64100(void);
void func_08A63FEC(A6Chain8 *self);

void func_08A63BF4(A6Chain8 *self) {
    func_08A64100();
    func_08A63FEC(self);
    A6Entry8 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}
}
