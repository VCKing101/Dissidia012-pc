typedef struct Owner0883E75C {
    int field_00;
    int field_04;
    void *handle;
} Owner0883E75C;

void func_08842138(void *p);

void func_0883E75C(Owner0883E75C *self) {
    if (self->handle != 0) {
        func_08842138(self->handle);
        self->handle = 0;
    }
    func_08842138(self);
}
