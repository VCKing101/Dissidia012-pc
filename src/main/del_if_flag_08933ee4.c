/* Deleting-destructor shell: when self is non-null and bit 0 of flags is set, calls func_08992F04 on it. */
void func_08992F04(void *p);

void func_08933EE4(void *self, int flags) {
    flags = flags & 1;
    if (self != 0) {
        if (flags) { func_08992F04(self); }
    }
}
