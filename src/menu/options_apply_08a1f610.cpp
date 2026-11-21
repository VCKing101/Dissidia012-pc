#include "menu/agent36_types.h"

/* Menu root: takes a copy of the options block into the second effect
 * block and hands its mode flag on. */
struct RootOptionsCopy {
    static void Copy(RootOptions36 *to, RootOptions36 *from) { *to = *from; }
};

extern "C" void func_08A1F610(MenuRoot36 *self, RootOptions36 *options)
{
    int none = 0;

    func_08A1F1DC(self);
    func_088AEFA0(D_089D7260);
    RootOptionsCopy::Copy(&self->effect2->options, options);
    int on = 0;
    if (self->effect2->options.field_0E == 1) {
        on = 1;
    }
    Kept<int> kept(on);
    func_088AF044(D_089D7260);
    func_08B58640(self->heap, kept.value);
    func_08B58538(none);
}
