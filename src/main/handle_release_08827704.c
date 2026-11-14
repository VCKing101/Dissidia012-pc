typedef struct Owner08827704 {
    unsigned char unknown_000[0x594];
    int handle;
} Owner08827704;

void func_08842138(int handle);

void func_08827704(Owner08827704 *self) {
    if (self->handle != 0) {
        func_08842138(self->handle);
        self->handle = 0;
    }
}
