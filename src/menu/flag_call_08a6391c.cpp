struct A6Entry7 {
    short delta;
    void (*fn)(char *);
};

struct A6Table7 {
    char pad_00[0xA0];
    A6Entry7 entry;
};

struct A6Flags2 {
    char field_00;
    char field_01;
    char pad_02[0xA];
    A6Table7 *field_0C;
};

extern "C" {
void func_08A6391C(A6Flags2 *self) {
    self->field_00 = 1;
    A6Entry7 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}

void func_08A63950(A6Flags2 *self) {
    self->field_00 = 0;
    A6Entry7 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}

void func_08A63980(A6Flags2 *self) {
    self->field_01 = 1;
    A6Entry7 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}

void func_08A639B4(A6Flags2 *self) {
    self->field_01 = 0;
    A6Entry7 *e = &self->field_0C->entry;
    e->fn((char *)self + e->delta);
}
}
