/* Returns the first or second byte of the pair at D_09B6DF08 + 0x148, or 0
   while func_08AB6DAC reports the menu busy.
   Retail keeps `self` in the frame (0($sp)) across the guard call and
   `second` in s0. The Kept local gives the frame slot; the named copy
   `which` after it puts the slot store before the s0 copy, as retail has. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Menu08AB456C;

struct Table08AB456C {
    char unknown_000[0x148];
    signed char pair[2];
};

extern "C" {

extern Table08AB456C D_09B6DF08;
int func_08AB6DAC(Menu08AB456C *menu);
void *func_08AB6D58(Menu08AB456C *menu);

int func_08AB456C(Menu08AB456C *self, int second)
{
    Kept<Menu08AB456C *> saved(self);
    int which = second;
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return 0;
    }
    func_08AB6D58(self);
    signed char *pair = D_09B6DF08.pair;
    if (which == 0) {
        return pair[0];
    } else {
        return pair[1];
    }
}

}
