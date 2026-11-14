/* Releases the node's handle at +0x20 and frees its block at +0x1C from the
   heap func_088908E0 returns, then marks both empty.
   Retail keeps `self` in the frame (0($sp)) across the three calls and reloads
   it where both paths join: a local object with a constructor (Kept) is never
   promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Node08887B4C {
    char unknown_00[0x1C];
    void *block;
    int handle;
};

extern "C" {

extern void *D_089D6C80;
void func_088A0DFC(void *registry, int handle);
void *func_088908E0(void);
void func_0885C538(void *heap, void *data, int flags);

void func_08887B4C(Node08887B4C *self)
{
    if (self->block != 0) {
        Kept<Node08887B4C *> saved(self);
        func_088A0DFC(D_089D6C80, self->handle);
        void *heap = func_088908E0();
        func_0885C538(heap, saved.value->block, 0);
        self = saved.value;
    }
    self->handle = -1;
    self->block = 0;
}

}
