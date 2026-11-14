#include "menu/auto_region1.h"

/* Menu root: plays script on texts with its messages. */
void func_08A1D89C(MenuRoot *self, void *texts, const unsigned short *script)
{
    func_088D3F7C(self->work->messages, texts, script, 1);
}
