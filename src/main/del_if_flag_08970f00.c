/* Deleting-destructor shell: when self is non-null and bit 0 of flags is set, calls func_089710C4 on it. */
void func_089710C4(void *p);

void func_08970F00(void *self, int flags) {
    flags = flags & 1;
    if (self != 0) {
        if (flags) { func_089710C4(self); }
    }
}
