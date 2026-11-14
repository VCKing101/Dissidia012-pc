extern void func_08992F04(void *object);

void func_08A500EC(void *self, unsigned int flags) {
    if (self != 0 && (flags & 1) != 0) {
        func_08992F04(self);
    }
}
