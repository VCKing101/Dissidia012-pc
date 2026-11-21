/* Deleting destructor of the page with vtable D_08A3B718: destroys its
 * sixteen 0x90-byte slots, then the base (func_08A408B4). */
extern "C" {
void func_0899372C(void *array, int count, int size, void (*dtor)(void), int a, int b); /* array destructor */
void func_08A3A5D8(void);
void func_08A408B4(void *self, int flags);
void func_08A40920(void *self);
extern char D_08A3B718[];
}

/* Member at +4 whose destructor destroys its 16 entries of 0x90 bytes. */
struct Slots3A628 {
    char pad_000[0x490];
    char entries[0x10][0x90];

    static void Destroy(Slots3A628 *slots)
    {
        if (slots != 0) {
            func_0899372C(slots->entries, 0x10, 0x90, func_08A3A5D8, 0, 0);
        }
    }
};

struct Page3A628 {
    void *vtable;
    Slots3A628 slots;

    static void Destroy(Page3A628 *self, unsigned int flags, void *vtable)
    {
        if (self != 0) {
            self->vtable = vtable;
            Slots3A628::Destroy(&self->slots);
            func_08A408B4(self, 0);
            if (flags & 1) {
                func_08A40920(self);
            }
        }
    }
};

extern "C" void func_08A3A628(Page3A628 *self, unsigned int flags)
{
    Page3A628::Destroy(self, flags, D_08A3B718);
}
