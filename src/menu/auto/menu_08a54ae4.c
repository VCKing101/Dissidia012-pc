#include "menu/auto_region2.h"

/* Widget54E98: plays its layout script and hands its text to its handles. */
void func_08A54AE4(Widget54E98 *self)
{
    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, self->text_script);
}
