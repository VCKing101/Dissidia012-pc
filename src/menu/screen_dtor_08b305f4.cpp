/* Deleting destructor: restores the vtable and frees the object when asked. */
extern "C" {
extern int D_08B310E8;
void func_08992F04(void *block);
}

struct Screen08B305F4 {
    char pad_000[0x9DC];
    void *vtable;

    static void destroy(Screen08B305F4 *self, int flags, void *vtable)
    {
        if (self != 0) {
            self->vtable = vtable;
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08B305F4(Screen08B305F4 *self, int flags)
{
    Screen08B305F4::destroy(self, flags, &D_08B310E8);
}
}
