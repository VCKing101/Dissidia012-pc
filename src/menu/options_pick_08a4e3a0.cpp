#include "menu/agent36_types.h"

struct IdE3A0 {
    int low;
    int high;

    bool IsNone(void) const
    {
        if ((low & high) == -1) {
            return true;
        } else {
            return false;
        }
    }
};

struct RefE3A0 {
    int key;                  /* +0x00 */
};

struct WorkE3A0 {
    char pad_0000[0x4];
    int state;                /* +0x0004 */
    char pad_0008[0x4];
    void *messages;           /* +0x000C */
    char pad_0010[0x6FD8];
    void *list;               /* +0x6FE8 */
};

struct ScreenE3A0 {
    char pad_00[0x3C];
    void *field_3C;           /* +0x3C */
    char pad_40[0x1C];
    WorkE3A0 *work;           /* +0x5C */
};

extern "C" {
void func_08A4D6D8(void);
IdE3A0 func_08928E70(int key);
void func_08A5B9A8(void *list, void *a, void *messages, RefE3A0 *ref);
void func_08A4D07C(ScreenE3A0 *self, int a, float b);
void func_08A4E2E4(ScreenE3A0 *self, int kind, void (*a)(void), void (*b)(void), void (*c)(void), int d, int e, int f, int g);
}

extern "C" bool func_08A4E3A0(ScreenE3A0 *self, RefE3A0 *ref)
{
    IdE3A0 id = func_08928E70(ref->key);

    if (!id.IsNone()) {
        func_08A4E2E4(self, 2, func_08A4D6D8, func_08A4D6D8, func_08A4D6D8, id.low, -1, -1, -1);
        return false;
    }
    func_08A5B9A8(self->work->list, self->field_3C, self->work->messages, ref);
    func_08A4D07C(self, 0, 0.0f);
    self->work->state = 10;
    return true;
}
