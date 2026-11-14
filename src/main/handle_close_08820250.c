typedef struct Owner08820250 {
    int field_00;
    int handle;
} Owner08820250;

void func_08842138(int handle);
void func_08820240(Owner08820250 *self);

void func_08820250(Owner08820250 *self) {
    if (self->handle != 0) {
        func_08842138(self->handle);
    }
    func_08820240(self);
}
