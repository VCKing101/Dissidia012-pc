/* Polls the task's wait object at +0x1C and moves the task to state 3 when it
   reports done. Always returns 0.
   Retail keeps `self` in the frame (0($sp)) across the call: a local object
   with a constructor (Kept) is never promoted by SNC -O2, and copying it back
   right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Task089F06E8 {
    int unknown_00;
    int unknown_04;
    int state;
};

extern "C" {

int func_089EF80C(void *wait);

int func_089F06E8(Task089F06E8 *self)
{
    Kept<Task089F06E8 *> saved(self);
    int done = func_089EF80C((char *)self + 0x1C);
    self = saved.value;
    if (done == 1) {
        self->state = 3;
    }
    return 0;
}

}
