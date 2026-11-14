#include "menu/auto_region2.h"

/* MenuRoot2: looks up message id (kind 0). */
char *func_08A412DC(MenuRoot2 *self, int id)
{
    return func_0887B500(self->work->messages, 0, id);
}
