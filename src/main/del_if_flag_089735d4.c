/* Deleting-destructor shell: when self is non-null and bit 0 of flags is set, calls func_089748D0 on it. */
void func_089748D0(void *p);

void func_089735D4(void *self, int flags) {
    flags = flags & 1;
    if (self != 0) {
        if (flags) { func_089748D0(self); }
    }
}
