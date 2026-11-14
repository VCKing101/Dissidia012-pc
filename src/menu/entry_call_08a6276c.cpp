struct A6Entry6 {
    short delta;
    void (*fn)(char *);
};

struct A6Table6 {
    char pad_00[0x50];
    A6Entry6 entry;
};

struct A6Owner6 {
    char pad_00[0x24];
    A6Table6 *field_24;
};

extern "C" {
void func_08A6276C(A6Owner6 *self) {
    A6Entry6 *e = &self->field_24->entry;
    e->fn((char *)self + e->delta);
}
}
