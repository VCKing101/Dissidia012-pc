extern "C" {
extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short picture);
}

struct Owner462F4 { int pad[0x10]; void *entry40; };
struct Win462F4 { char pad[0x418]; Owner462F4 **owner; };

/* Inlined call on the loader in D_089D7830. */
struct Loader9D7830 {
    static void Picture(void *entry, char kind, unsigned short picture) {
        func_088D0984(D_089D7830, entry, kind, picture);
    }
};

extern "C" void func_08A462F4(Win462F4 *self, int id)
{
    Loader9D7830::Picture((*self->owner)->entry40, 0x11, id + 0x4035);
}
