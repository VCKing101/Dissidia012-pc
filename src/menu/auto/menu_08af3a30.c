#include "menu/auto_region3.h"

/* ScreenB01CF8: message id of any kind. */
void func_08AF3A30(ScreenB01CF8 *self, int kind, int id)
{
    func_0887B500(self->work->messages, kind, id);
}
