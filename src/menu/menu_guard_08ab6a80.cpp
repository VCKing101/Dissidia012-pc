/* Returns a 64-bit value: D_08AB7C88 while func_08AB6DAC reports the menu
   busy, otherwise func_08A706F0 of the table at D_09B6DF08 + 0x148.
   Retail keeps `self` in the frame (0($sp)) across the guard call: a local
   object with a constructor (Kept) is never promoted by SNC -O2, and copying
   it back right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Menu08AB6A80;

struct Table08AB6A80 {
    char unknown_000[0x148];
    signed char pair[2];
};

extern "C" {

extern Table08AB6A80 D_09B6DF08;
extern long long D_08AB7C88;
int func_08AB6DAC(Menu08AB6A80 *menu);
void *func_08AB6D58(Menu08AB6A80 *menu);
long long func_08A706F0(void *table);

long long func_08AB6A80(Menu08AB6A80 *self)
{
    Kept<Menu08AB6A80 *> saved(self);
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return D_08AB7C88;
    }
    func_08AB6D58(self);
    return func_08A706F0(D_09B6DF08.pair);
}

}
