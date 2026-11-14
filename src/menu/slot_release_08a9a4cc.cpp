/* Releases the object at +0x92C of the menu's data block, frees the buffer at
   +0x928 from the menu heap and clears both.
   Retail keeps `self` in the frame (0($sp)) across both calls: a local object
   with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Data08A9A4CC {
    char unknown_000[0x928];
    void *buffer;
    void *object;
};

struct Menu08A9A4CC {
    char unknown_000[0x138];
    void *heap;
    char unknown_13C[0x18];
    Data08A9A4CC *data;
};

extern "C" {

void func_08A80D40(void *object, int mode);
void func_0885C538(void *heap, void *data, int flags);

void func_08A9A4CC(Menu08A9A4CC *self)
{
    void *object = self->data->object;
    if (object != 0) {
        Kept<Menu08A9A4CC *> saved(self);
        func_08A80D40(object, 2);
        self = saved.value;
        func_0885C538(self->heap, self->data->buffer, 0);
        self = saved.value;
        self->data->object = 0;
        self->data->buffer = 0;
    }
}

}
