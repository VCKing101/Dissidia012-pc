typedef struct Owner0881F49C {
    int field_00;
    int handle;
} Owner0881F49C;

void func_08842138(int handle);
void func_0881F47C(Owner0881F49C *self);

void func_0881F49C(Owner0881F49C *self) {
    if (self->handle != 0) {
        func_08842138(self->handle);
    }
    func_0881F47C(self);
}
