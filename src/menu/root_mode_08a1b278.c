#include "menu/agent29_types.h"

/* Starts a menu mode on a screen with its task at +0x100: records the
   mode (7: 1, or 2 when the previous mode was 2; 8: 0 with the return
   flag) and `arg` in the request block D_09B6D840, then restarts the
   task with the root's callbacks. */
typedef struct RootRequest29 {
    int state;
    int previous;
    int value;
    char pad_0C[0x4];
    int field_10;
    int field_14;
    char pad_18[0x4];
    int arg;            /* +0x1C */
    unsigned char back; /* +0x20 */
} RootRequest29;

extern RootRequest29 D_09B6D840;
extern void func_08A1B3CC(void);
extern void func_08A1B3EC(void);

void func_08A1B278(char *self, int mode, int arg)
{
    RootRequest29 *request = &D_09B6D840;
    MenuTask *task;

    request->back = 0;
    if (mode == 7) {
        mode = request->previous == 2 ? 2 : 1;
    } else if (mode == 8) {
        mode = 0;
        request->back = 1;
    } else {
        request->field_10 = 0;
        request->field_14 = 0;
    }
    D_09B6D840.state = mode;
    request->previous = mode;
    request->arg = arg;
    task = (MenuTask *)(self + 0x100);
    func_0886503C(task, func_08A1B3CC, func_08A1B3EC, 0);
    func_088650A4(task);
    func_0886504C(task, self, 0x100);
}
