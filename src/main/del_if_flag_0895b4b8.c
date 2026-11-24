/* Deleting-destructor shell: when self is non-null and bit 0 of flags is set, calls func_0895B7DC on it. */
void func_0895B7DC(void *p);

void func_0895B4B8(void *self, int flags) {
    flags = flags & 1;
    if (self != 0) {
        if (flags) { func_0895B7DC(self); }
    }
}
