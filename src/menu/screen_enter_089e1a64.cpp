#include "menu/agent29_types.h"

/* ScreenE75C4 enter: clears its texts, sets up the pieces it shares with
   ScreenE75C8 and the layout, then starts in state 0 (or 0x11 when
   func_0887059C reports D_089D5FB4 ready). */
extern "C" {
void func_089E2D58(ScreenE75C4 *self);
void func_08A2B07C(ScreenE75C8 *screen);
unsigned char func_089E3BFC(ScreenE75C4 *self);
void func_088850EC(void *object, char kind, const void *data);
void func_08885340(void *object, char value);
int func_0887059C(void *object);
extern const unsigned char D_089E73D0[];
}

/* Inlined setter on the object in D_089D69E8: the kind argument is set
   before the global is loaded. */
struct Object9D69E8 {
    static void Set(char kind, const void *data) {
        func_088850EC(D_089D69E8, kind, data);
    }
};

extern "C" void func_089E1A64(ScreenE75C4 *self)
{
    int i;
    int none = 0;

    for (i = 0; i < 0x59; i++) {
        self->work->texts[i] = 0;
    }
    func_088F0A78(1);
    func_089E2D58(self);
    func_08A2B07C(D_089E75C8);
    func_089E3BFC(self);
    Object9D69E8::Set(6, D_089E73D0);
    func_08885340(D_089D69E8, none);
    func_088D38F0(&self->work->layout);
    func_088D3B5C(self->work->handles, 0x120);
    self->work->state = func_0887059C(D_089D5FB4) != 0 ? 0x11 : 0;
    WORK_INT(self->work, 0xA98) = 0;
}
