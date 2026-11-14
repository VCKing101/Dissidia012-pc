#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): when the course list reports done (and, in
   the stream or arcade modes, the confirm mask is pressed), starts the
   close animation, stops the music and moves to state 3. */
extern "C" {
extern unsigned char D_089D5F1F;
extern unsigned char D_089D5F20;
extern unsigned long long D_089E9CF0;
int func_089EA150(void *list);
void func_089E9348(ScreenE9D38Ex *self, int time, void (*done)(void));
void func_089E94A0(void);

void func_089E90D8(ScreenE9D38Ex *self)
{
    int done = func_089EA150(self->field_144);

    if (done == 1 && (D_089D5F1F == 1 || D_089D5F20 == 1)) {
        if (func_088729A0(0)->Pressed(D_089E9CF0) == 0) {
            done = 0;
        }
    }
    if (done == 1) {
        func_089E9348(self, func_088D4C5C() * 3, func_089E94A0);
        func_088D54DC(&self->sound);
        self->field_191 = 0;
        self->field_170 = 3;
    }
}
}
