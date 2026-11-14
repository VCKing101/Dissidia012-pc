/* Releases the held block, or just forgets the pending one. */
template <class T> struct Kept088AB330 {
    T value;
    Kept088AB330(T v) : value(v) {}
};

struct Holder088AB330 {
    char pad_00[0x10];
    void *block;
    void *pending;
};

extern "C" {

void func_088A51E4(void *block);

void func_088AB330(Holder088AB330 *self)
{
    void *block = self->block;
    if (block != 0) {
        Kept088AB330<Holder088AB330 *> saved(self);
        func_088A51E4(block);
        self = saved.value;
        self->block = 0;
    } else if (self->pending != 0) {
        self->pending = 0;
    }
}

}
