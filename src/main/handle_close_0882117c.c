typedef struct Owner0882117C {
    unsigned char unknown_00[0xC];
    int handle;
} Owner0882117C;

void func_08842138(int handle);
void func_08821168(Owner0882117C *self);

void func_0882117C(Owner0882117C *self) {
    if (self->handle != 0) {
        func_08842138(self->handle);
    }
    func_08821168(self);
}
