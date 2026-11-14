/* Releases and frees a buffer, then clears its three fields.
   Retail keeps `buffer` in the frame (0($sp)) across both calls: a local
   object with a constructor (Kept) is never promoted by SNC -O2. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Buffer0889EFF8 {
    void *data;
    int used;
    int capacity;
};

extern "C" {

void func_088A0D1C(void *data);
void func_088A0CD4(void *data);

void func_0889EFF8(Buffer0889EFF8 *buffer)
{
    void *data = buffer->data;
    if (data != 0) {
        Kept<Buffer0889EFF8 *> saved(buffer);
        func_088A0D1C(data);
        func_088A0CD4(saved.value->data);
        Buffer0889EFF8 *b = saved.value;
        b->data = 0;
        b->used = 0;
        b->capacity = 0;
    }
}

}
