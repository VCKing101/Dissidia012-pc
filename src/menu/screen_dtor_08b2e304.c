/* Deleting destructor: restores the vtable, clears the screen singleton and
   frees the object when asked. */
struct Screen08B2E304 {
    char pad_000[0x848];
    void *vtable;
};

extern int D_08B2F500;
extern struct Screen08B2E304 *D_08B2F640;
extern void func_08992F04(void *block);

void func_08B2E304(struct Screen08B2E304 *self, int flags)
{
    if (self != 0) {
        self->vtable = &D_08B2F500;
        D_08B2F640 = 0;
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
