/* Deleting destructor of an effect: resets its listener base. */
extern "C" {
extern char D_089CB188[];
void func_088A7E48(void *p);
}

struct Listener088A68B8 {
    void *vtable;

    static void destroy(Listener088A68B8 *l)
    {
        if (l != 0) {
            l->vtable = D_089CB188;
        }
    }
};

struct Fx088A68B8 {
    char pad_000[0x110];
    Listener088A68B8 listener;
};

extern "C" {

void func_088A68B8(Fx088A68B8 *fx, unsigned int flags)
{
    if (fx != 0) {
        Listener088A68B8::destroy(&fx->listener);
        if (flags & 1) {
            func_088A7E48(fx);
        }
    }
}

}
