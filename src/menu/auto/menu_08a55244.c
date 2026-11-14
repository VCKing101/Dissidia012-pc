#include "menu/auto_region2.h"

/* Item: plays its layout script and hands its texts to its handles. */
void func_08A55244(Item *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, self->text_script);
}
