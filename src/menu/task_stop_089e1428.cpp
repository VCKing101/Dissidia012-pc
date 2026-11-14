#include "menu/auto_region1.h"

/* ScreenE75C4: stops its task unless func_0887E718 reports the shared
   object busy. Retail keeps `self` in the frame across the first call. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {
int func_0887E718(void *object);

void func_089E1428(ScreenE75C4 *self)
{
    Kept<ScreenE75C4 *> saved(self);
    int busy = func_0887E718(D_089D6998);
    self = saved.value;
    if (busy != 0) {
        return;
    }
    func_08865158(&self->task);
}
}
