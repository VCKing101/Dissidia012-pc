#include "menu/agent36_types.h"

struct IdDC18 {
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

struct RefDC18 {
    void *head;               /* +0x00 */
    void *body;               /* +0x04 */
};

struct WorkDC18 {
    unsigned int flags;       /* +0x0000 */
    int state;                /* +0x0004 */
    char pad_0008[0x6B18];
    char *save;               /* +0x6B20 */
    char pad_6B24[0x4C8];
    void *slider;             /* +0x6FEC */
};

struct ScreenDC18 {
    char pad_00[0x5C];
    WorkDC18 *work;           /* +0x5C */
};

extern "C" {
void func_08A4D6D8(void);
IdDC18 func_089289B4(const void *head);
void func_08935524(void *dst, const void *src, int size);
void func_08A66478(void *slider, RefDC18 *ref);
void func_08A4D07C(ScreenDC18 *self, int a, float b);
void func_08A4E2E4(ScreenDC18 *self, int kind, void (*a)(void), void (*b)(void), void (*c)(void), int d, int e, int f, int g);
}

extern "C" bool func_08A4DC18(ScreenDC18 *self, RefDC18 *ref)
{
    IdDC18 id = func_089289B4(ref->head);

    if (!id.IsNone()) {
        func_08A4E2E4(self, 2, func_08A4D6D8, func_08A4D6D8, func_08A4D6D8, id.low, -1, -1, -1);
        return false;
    }
    if (Flags36::Has(self->work->flags, 0x20)) {
        self->work->flags &= ~0x20;
    } else {
        func_08935524(self->work->save, ref->head, 0x160);
        func_08935524(self->work->save + 0x160, ref->body, 0x3FC);
    }
    func_08A66478(self->work->slider, ref);
    func_08A4D07C(self, 0, 0.0f);
    self->work->state = 0x12;
    return true;
}
