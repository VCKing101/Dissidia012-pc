/* Frees a buffer and clears its pointer and size.
   Retail keeps `buffer` in the frame (0($sp)) across the call: a local
   object with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Buffer08895AE4 {
    void *data;
    int size;
};

extern "C" {

void func_088A51E4(void *data);

void func_08895AE4(Buffer08895AE4 *buffer)
{
    void *data = buffer->data;
    if (data != 0) {
        Kept<Buffer08895AE4 *> saved(buffer);
        func_088A51E4(data);
        Buffer08895AE4 *b = saved.value;
        b->data = 0;
        b->size = 0;
    }
}

}
