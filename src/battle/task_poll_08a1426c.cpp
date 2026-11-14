/* Polls the task's wait object at +0x178 and moves the task to state 8 when it
   reports done. Always returns 0.
   Retail keeps `self` in the frame (0($sp)) across the call: a local object
   with a constructor (Kept) is never promoted by SNC -O2, and copying it back
   right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Task08A1426C {
    int unknown_00;
    int unknown_04;
    int state;
};

extern "C" {

int func_089F6388(void *wait);

int func_08A1426C(Task08A1426C *self)
{
    Kept<Task08A1426C *> saved(self);
    int done = func_089F6388((char *)self + 0x178);
    self = saved.value;
    if (done == 1) {
        self->state = 8;
    }
    return 0;
}

}
