/* Polls the page: 1 when idle and ready, -1 when the state is 2 or 4, else 0.
   Retail keeps `self` in the frame across the first call (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct PollPage;

extern "C" int func_08A1D90C(PollPage *self);
extern "C" int func_08A1D8E4(PollPage *self);

extern "C" {
int func_08A1D9E4(PollPage *self)
{
    Kept<PollPage *> saved(self);
    int result = 0;
    int state = func_08A1D90C(self);
    self = saved.value;
    if (state != 0) {
        if (state == 2 || state == 4) {
            result = -1;
        }
    } else if (func_08A1D8E4(self) != 0) {
        result = 1;
    }
    return result;
}
}
