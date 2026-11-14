#include "menu/auto_region2.h"

/* Item: hides texts 1, 2, 4, 5, 7, 8, 10 and 11. */
void func_08A55878(Item *self)
{
    func_0887B944(self->texts[1]);
    func_0887B944(self->texts[2]);
    func_0887B944(self->texts[4]);
    func_0887B944(self->texts[5]);
    func_0887B944(self->texts[7]);
    func_0887B944(self->texts[8]);
    func_0887B944(self->texts[10]);
    func_0887B944(self->texts[11]);
}
