#include "menu/auto_region1.h"

extern const unsigned short D_08A09C22[];
extern const unsigned short D_08A09C3A[];

/* Screen0B6A0: plays D_08A09C22 and makes texts 0x16, 0x17 and 0x20 from
 * the messages that func_089EF640 / EF700 / EF748 pick for field_3158. */
void func_089F490C(Screen0B6A0 *self)
{
    int id;

    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09C22);
    id = func_089EF640(self, &self->work->field_3158);
    if (id != 0xFFFF) {
        self->work->texts[0x16] = func_089EF1E8(self, 0, id, 0, 0);
    }
    id = func_089EF700(self, &self->work->field_3158);
    if (id != 0xFFFF) {
        self->work->texts[0x17] = func_089EF1E8(self, 0, id, 0, 0);
    }
    id = func_089EF748(self, &self->work->field_3158);
    if (id != 0xFFFF) {
        self->work->texts[0x20] = func_089EF1E8(self, 0, id, 0, 0);
        self->work->texts[0x21] = func_0887E534(self->work->texts[0x20]);
    }
    func_088D428C(self->work->handles, self->work->texts, D_08A09C3A, 1);
    func_089F3F68(self, 0xA5);
    func_089F3F68(self, 0xAA);
}
