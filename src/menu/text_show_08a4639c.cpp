extern "C" {
extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short picture);
}

struct Owner4639C { int pad[0x11]; void *entry44; };
struct Win4639C { char pad[0x418]; Owner4639C **owner; };

/* Inlined call on the loader in D_089D7830. */
struct Loader9D7830 {
    static void Picture(void *entry, char kind, unsigned short picture) {
        func_088D0984(D_089D7830, entry, kind, picture);
    }
};

extern "C" void func_08A4639C(Win4639C *self, unsigned short id)
{
    Loader9D7830::Picture((*self->owner)->entry44, 0x1B, id);
}
