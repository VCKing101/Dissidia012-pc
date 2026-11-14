#include "menu/agent29_types.h"

/* ScreenE75C4: checks the two button masks; the first confirms the request
   (result 0x11, state 4), the second opens page 1 (state 0xD). Returns 1
   when either was pressed. */
extern "C" {
extern unsigned long long D_089E7288;
extern unsigned long long D_089E7290;

int func_089E2B40(ScreenE75C4 *self)
{
    int page;

    if (func_088729A0(0)->Pressed(D_089E7288) != 0) {
        func_088D4BF4(1);
        D_09B6D830.result = 0x11;
        self->work->state = 4;
        return 1;
    } else if (func_088729A0(0)->Pressed(D_089E7290) != 0) {
        page = 1;
        func_088D4BF4(1);
        WORK_INT(self->work, 0xB1C) = page;
        self->work->state = 0xD;
        return 1;
    }
    return 0;
}
}
