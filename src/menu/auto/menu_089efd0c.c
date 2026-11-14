#include "menu/auto_region1.h"

/* Declared here rather than in the header: this caller passes a char (the
 * value is sign-extended again), func_089F73AC passes an int as it is. */
extern unsigned short func_089EF880(Screen0B6A0 *self, char value);

/* Screen0B6A0: looks up slot index's value and hands it to func_089EFB50. */
void func_089EFD0C(Screen0B6A0 *self, int index)
{
    int value;

    value = func_089EF880(self, self->work->slots[index].field_00);
    func_089EFB50(self, value, index);
}
