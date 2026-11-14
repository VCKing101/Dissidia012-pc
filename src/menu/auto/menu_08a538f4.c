#include "menu/auto_region2.h"

/* Widget54438: plays its scripts, then updates its two elements. */
void func_08A538F4(Widget54438 *self)
{
    int i;

    func_088D3F00(0, self->handles, self->script);
    func_088D44EC(self->handles, self->texts, self->text_script);
    for (i = 0; i < 2; i++) {
        func_08A52EBC(&self->field_E0[i]);
    }
}
