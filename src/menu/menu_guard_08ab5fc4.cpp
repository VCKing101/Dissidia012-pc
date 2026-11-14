/* Flag tests guarded by func_08AB6DAC: true while the menu is busy, otherwise
   whether flag 0x200 (func_08AB5FC4) or 0x400 (func_08AB6010) is set at +0x2C.
   Retail keeps `self` in the frame (0($sp)) across the guard call: a local
   object with a constructor (Kept) is never promoted by SNC -O2, and copying
   it back right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Menu08AB5FC4 {
    char unknown_00[0x2C];
    unsigned flags;
};

extern "C" {

int func_08AB6DAC(Menu08AB5FC4 *menu);

bool func_08AB5FC4(Menu08AB5FC4 *self)
{
    Kept<Menu08AB5FC4 *> saved(self);
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return true;
    }
    if ((self->flags & 0x200) != 0) {
        return true;
    }
    return false;
}

bool func_08AB6010(Menu08AB5FC4 *self)
{
    Kept<Menu08AB5FC4 *> saved(self);
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return true;
    }
    if ((self->flags & 0x400) != 0) {
        return true;
    }
    return false;
}

}
