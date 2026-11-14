#include "menu/auto_region3.h"

/* PanelC18: keeps the layout, handles and messages it draws with. */
void func_08AC88B8(PanelC18 *self, MenuLayout *layout, MenuModel **handles, void *messages)
{
    self->work->layout = layout;
    self->work->handles = handles;
    self->work->messages = messages;
}
