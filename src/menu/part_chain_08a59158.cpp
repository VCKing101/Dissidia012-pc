struct A6Entry4 {
    short delta;
    void (*fn)(char *);
};

struct A6Table4 {
    char pad_00[8];
    A6Entry4 entry;
};

struct A6Chain4 {
    char pad_00[8];
    A6Table4 *field_08;
};

extern "C" {
void func_08A597B0(void);
void func_08A59680(A6Chain4 *self);

void func_08A59158(A6Chain4 *self) {
    func_08A597B0();
    func_08A59680(self);
    A6Entry4 *e = &self->field_08->entry;
    e->fn((char *)self + e->delta);
}
}
