#include "menu/auto_region4.h"

void func_08B39E40(ScreenB41CC0 *self, int id)
{
    int result;

    self->field_C4->field_28 = id;
    func_089246C4(id);
    result = func_089233C4(id);
    if (result != 0) {
        result = func_08B46A00(self->field_C4->field_10);
        if (result != 0) {
            func_089246C4(result);
        }
    }
}
