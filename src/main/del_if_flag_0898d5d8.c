/* Deleting-destructor shell: when self is non-null and bit 0 of flags is set, calls func_0898E938 on it. */
void func_0898E938(void *p);

void func_0898D5D8(void *self, int flags) {
    flags = flags & 1;
    if (self != 0) {
        if (flags) { func_0898E938(self); }
    }
}
