#include "menu/auto_region2.h"

/* Widget52C98: plays its layout script and hands its texts to its handles. */
void func_08A5287C(Widget52C98 *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, self->text_script);
}
