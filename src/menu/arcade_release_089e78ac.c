#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu) leave: destroys its three parts, then the
   heap and the heap's buffer. */
void func_089E78AC(ScreenE9D38 *screen)
{
    ScreenE9D38Ex *self = (ScreenE9D38Ex *)screen;

    if (self->field_13C != 0) {
        void *part = self->field_13C;
        if (part != 0) {
            func_0887B1F4(part, 2);
        }
        func_0885C538(self->heap, self->field_13C, 0);
        self->field_13C = 0;
    }
    if (self->field_140 != 0) {
        void *part = self->field_140;
        if (part != 0) {
            func_0887B1F4(part, 2);
        }
        func_0885C538(self->heap, self->field_140, 0);
        self->field_140 = 0;
    }
    if (self->field_144 != 0) {
        func_0885C538(self->heap, self->field_144, 0);
        self->field_144 = 0;
    }
    func_0885C488(self->heap);
    func_0888208C(self->buffer);
    self->heap = 0;
    self->buffer = 0;
}
