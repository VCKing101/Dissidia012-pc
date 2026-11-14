/* Deleting destructor: restores the vtable, clears the screen singleton and
   frees the object when asked. */
struct Screen08B2B7E4 {
    char pad_000[0xA1C];
    void *vtable;
};

extern int D_08B2CB80;
extern struct Screen08B2B7E4 *D_08B2D264;
extern void func_08992F04(void *block);

void func_08B2B7E4(struct Screen08B2B7E4 *self, int flags)
{
    if (self != 0) {
        self->vtable = &D_08B2CB80;
        D_08B2D264 = 0;
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
