/* Closes an open entry: notifies func_08904E18, frees its buffer from the
   entry's heap and clears the open flag.
   Retail keeps `self` in one frame slot (0($sp)) and stores it again before
   the second call. One Kept local, reassigned before that call, gives the
   same store, reload, store, reload sequence. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Entry08A6FB68 {
    char unknown_000[0x208];
    void *buffer;
    void *heap;
    char unknown_210[0xC];
    unsigned char open;
};

extern "C" {

void func_08904E18(Entry08A6FB68 *entry, void *heap);
void func_0885C538(void *heap, void *data, int flags);

void func_08A6FB68(Entry08A6FB68 *self)
{
    if (self->open == 1) {
        Kept<Entry08A6FB68 *> saved(self);
        func_08904E18(self, self->heap);
        self = saved.value;
        void *buffer = self->buffer;
        if (buffer != 0) {
            saved.value = self;
            func_0885C538(self->heap, buffer, 0);
            self = saved.value;
            self->buffer = 0;
        }
        self->open = 0;
    }
}

}
