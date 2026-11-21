#include "menu/agent36_types.h"

/* Screen28560: closes the current kind once both of its models (handles
 * 0x5B and 0x5A) have stopped. Returns 0 while they still move, 1 when
 * there was no kind or it is now closed. */
extern "C" int func_08A253F8(Screen28560x *self, void *layout, Model36 **handles, void **texts)
{
    int kind = self->work->kind;
    int none = -1;

    if (kind == none) {
        return 1;
    }
    if (Flags36::Has(handles[0x5B]->field_20, 0x20) &&
        Flags36::Has(handles[0x5A]->field_20, 0x20)) {
        func_088D3EA0(layout, handles, D_08A28AE8[kind].close_script);
        func_08A24E20(self, handles, texts);
        self->work->kind = none;
        return 1;
    }
    return 0;
}
