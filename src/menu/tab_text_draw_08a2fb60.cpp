#include "menu/agent36_types.h"

/* Tabbed pages: redraws the long text (clearing the shown marks first when
 * reset is set) and its scroll arrows. */

/* Called with the row count, which it does not read. */
extern "C" void func_08A2F6F0(TabScreen36 *self, ListPage36 *page, int rows);

extern "C" void func_08A2FB60(TabScreen36 *self, ListPage36 *page, unsigned char reset)
{
    if (reset) {
        for (int i = 0; i < 0x16; i++) {
            self->work->shown[i] = 0xFFFF;
        }
    }
    func_088D4C64(&self->work->lines, self->work->texts, self->work->shown, 0, 0, 1);
    func_08A2F6F0(self, page, 7);
    if (self->work->field_6D2 != 0) {
        func_088D4A68(self->work->arrows, &self->work->lines, self->work->handles[0x23],
                      self->work->handles[0x25], 1, 0x14, 0);
    }
}
