/* Frees the pool's block when both pointers are set, then resets the pool
   through func_0881E1F0.
   Retail keeps `self` in the frame (0($sp)) across the free and reloads it
   where both paths join: a Kept local inside the branch, copied back into
   `self` after the call, gives exactly that. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Pool0881E200 {
    int unknown_00;
    void *first;
    void *second;
};

extern "C" {

void func_0885C538(void *heap, void *data, int flags);
void func_0881E1F0(Pool0881E200 *self);

void func_0881E200(Pool0881E200 *self)
{
    void *first = self->first;
    if (first != 0) {
        void *second = self->second;
        if (second != 0) {
            Kept<Pool0881E200 *> saved(self);
            func_0885C538(second, first, 0);
            self = saved.value;
        }
    }
    func_0881E1F0(self);
}

}
