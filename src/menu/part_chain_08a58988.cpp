struct A6Entry3 {
    short delta;
    void (*fn)(char *);
};

struct A6Table3 {
    char pad_00[8];
    A6Entry3 entry;
};

struct A6Chain3 {
    char pad_00[8];
    A6Table3 *field_08;
};

extern "C" {
void func_08A58FD4(void);
void func_08A58EF4(A6Chain3 *self);

void func_08A58988(A6Chain3 *self) {
    func_08A58FD4();
    func_08A58EF4(self);
    A6Entry3 *e = &self->field_08->entry;
    e->fn((char *)self + e->delta);
}
}
