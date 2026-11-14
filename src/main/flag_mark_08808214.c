typedef struct Node08808214 {
    unsigned char unknown_0000[0x10];
    unsigned int flags;
} Node08808214;

extern unsigned char D_08B63280;
unsigned char func_088082CC(Node08808214 *node);

void func_08808214(Node08808214 *self) {
    if (D_08B63280 == 0) {
        return;
    }
    if (func_088082CC(self) == 0) {
        return;
    }
    self->flags |= 0x40;
}
