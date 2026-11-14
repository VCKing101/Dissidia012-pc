#include "menu/agent29_types.h"

/* Screen0B6A0: plays the pull's model scripts (the opening one, then the
   one for kind 1 or the other kinds, with their texts) and refreshes;
   kinds 0 and 3 have no models. */
extern const short D_08A0A822[];
extern const short D_08A0A872[];
extern const unsigned short D_08A0A8B2[];
extern const short D_08A0A8EA[];
extern const unsigned short D_08A0A942[];
extern void func_089FE114(Screen0B6A0 *self);
extern void func_089FD474(Screen0B6A0 *self);

void func_089FB97C(Screen0B6A0 *self)
{
    if (self->work->field_38A0 != 0) {
        if (self->work->field_38A0 != 3) {
            func_088D3F00(&self->work->layout, self->work->handles2, D_08A0A822);
            if (self->work->field_38A0 == 1) {
                func_088D3F00(&self->work->layout, self->work->handles2, D_08A0A872);
                func_088D44EC(self->work->handles2, self->work->texts, D_08A0A8B2);
            } else {
                func_088D3F00(&self->work->layout, self->work->handles2, D_08A0A8EA);
                func_088D44EC(self->work->handles2, self->work->texts, D_08A0A942);
            }
            func_089FE114(self);
            func_089FD474(self);
        }
    }
}
