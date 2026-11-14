#include "menu/agent29_types.h"

/* MenuRoot: unless a page is busy (func_08A1D8E4), resets the root, keeps
   `value` in the request block (D_09B6D840 +8), clears the page flags and
   opens the page with it. */
typedef struct RootRequest29 {
    int field_00;
    int field_04;
    int value;
} RootRequest29;

extern RootRequest29 D_09B6D840;
extern void func_08A1CB58(MenuRoot *self);
extern void func_088F8578(void *page, int value);

void func_08A1D964(MenuRoot *self, int value)
{
    RootRequest29 *request;

    if (func_08A1D8E4(self) == 0) {
        func_08A1CB58(self);
        request = &D_09B6D840;
        request->value = value;
        WORK_INT(self->work, 0x8E60) &= 0xFFFE4279;
        func_088F8578((void *)WORK_INT(self->work, 0x6964), request->value);
    }
}
