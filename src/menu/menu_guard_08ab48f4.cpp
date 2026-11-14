/* Returns the value at +0x210 of the menu's info block, or 0 while
   func_08AB6DAC reports the menu busy.
   Retail keeps `self` in the frame (0($sp)) across the guard call: a local
   object with a constructor (Kept) is never promoted by SNC -O2, and copying
   it back right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Menu08AB48F4;

struct Info08AB48F4 {
    char unknown_000[0x210];
    int value;
};

extern "C" {

int func_08AB6DAC(Menu08AB48F4 *menu);
Info08AB48F4 *func_08AB6D58(Menu08AB48F4 *menu);

int func_08AB48F4(Menu08AB48F4 *self)
{
    Kept<Menu08AB48F4 *> saved(self);
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return 0;
    }
    return func_08AB6D58(self)->value;
}

}
