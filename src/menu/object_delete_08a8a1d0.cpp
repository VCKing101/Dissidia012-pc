extern "C" {
void func_08992F04(void *self);

void func_08A8A1D0(void *self, unsigned flags) {
    if (self != 0 && (flags & 1) != 0) {
        func_08992F04(self);
    }
}
}
