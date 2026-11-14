#include "menu/auto_region1.h"

/* Screen28560: clears fields 30..38 of handle 0x5C and frees text 0x8F. */
void func_08A24E20(Screen28560 *self, MenuModel **handles, void **texts)
{
    MenuModel *model;

    model = handles[0x5C];
    model->field_34 = 0;
    model->field_30 = 0;
    model->field_38 = 0;
    if (texts[0x8F] != 0) {
        func_0887BA5C(texts[0x8F]);
        texts[0x8F] = 0;
    }
}
