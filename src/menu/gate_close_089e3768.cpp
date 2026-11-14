#include "menu/agent29_types.h"

/* ScreenE75C4: closes the selected gate once both of its door models
   (handles 0x40 and 0x3F) have stopped. Returns 0 while they still move,
   1 when there was no gate or it is now closed. */
extern "C" {

int func_089E3768(ScreenE75C4 *self, void *layout, void *handles, void *texts)
{
    int gate = self->work->field_B20;
    int none = -1;

    if (gate == none) {
        return 1;
    }
    if (!Flags29::Has(((MenuModel **)handles)[0x40]->field_20, 0x20) ||
        !Flags29::Has(((MenuModel **)handles)[0x3F]->field_20, 0x20)) {
        return 0;
    }
    func_088D3EA0(layout, handles, D_089E7434[gate].script);
    MenuModel *light = ((MenuModel **)handles)[0x41];
    light->field_34 = 0;
    light->field_30 = 0;
    light->field_38 = 0;
    if (((void **)texts)[0x51] != 0) {
        func_0887BA5C(((void **)texts)[0x51]);
        ((void **)texts)[0x51] = 0;
    }
    self->work->field_B20 = none;
    return 1;
}

}
