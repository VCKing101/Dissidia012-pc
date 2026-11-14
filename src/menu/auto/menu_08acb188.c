#include "menu/auto_region3.h"

extern const unsigned short D_08AD98AC[];
extern const unsigned short D_08AD98E4[];

/* ScreenDB30: makes its texts from D_08B01CF8's messages and plays D_08AD98AC. */
void func_08ACB188(ScreenDB30 *self)
{
    func_088D3F7C(((ScreenB01CF8 *)D_08B01CF8)->work->messages, self->work->texts, D_08AD98E4, 0);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AD98AC);
    func_08ACB1FC(self);
}
