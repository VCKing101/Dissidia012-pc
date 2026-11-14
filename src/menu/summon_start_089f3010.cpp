#include "menu/agent29_types.h"

extern "C" {
/* Screen0B6A0: starts a pull: clears +0x345C, plays the intro effect when
   func_089FCB18 asks for it, sets up the scene and moves to state 2. */
extern int func_089FCB18(Screen0B6A0 *self);
extern void func_089FAF14(Screen0B6A0 *self, int kind, int arg);
extern void func_08A065E0(Screen0B6A0 *self, int arg);
extern void func_089FB664(Screen0B6A0 *self);
extern void func_0888550C(void *object, void *messages, int arg1, int arg2);
extern void func_089FFC9C(Screen0B6A0 *self);
}

/* Inlined call on the object in D_089D69E8. */
struct Object9D69E8 {
    static void Show(void *messages) {
        func_0888550C(D_089D69E8, messages, 0, 0);
    }
};

extern "C" {

void func_089F3010(Screen0B6A0 *self)
{
    WORK_INT(self->work, 0x345C) = 0;
    if (func_089FCB18(self) != 0) {
        func_089FAF14(self, 0xC, -1);
    }
    func_08A065E0(self, 1);
    func_089FB664(self);
    Object9D69E8::Show(self->work->messages);
    self->work->state = 2;
    func_089FFC9C(self);
}
}
