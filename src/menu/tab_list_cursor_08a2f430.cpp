#include "menu/agent36_types.h"

struct CursorFlags36 {
    static void Set(Cursor36 *cursor, unsigned int flag) { cursor->flags |= flag; }
};

extern "C" void func_08A2F430(TabScreen36 *self, ListPage36 *page)
{
    int value = D_08A457EC->table->field_E70[page->entry];
    int pos = self->work->pos[page->list];
    int count = self->work->counts[page->list];
    int visible = count;

    if (visible >= 11) {
        visible = 10;
    }
    func_088D46E4(&self->work->cursor, value, 0, count - 1, pos, visible);
    CursorFlags36::Set(&self->work->cursor, 4);
    for (int i = 0; i < 0x16; i++) {
        self->work->shown[i] = 0xFFFF;
    }
    func_088D4C64(&self->work->cursor, self->work->texts, self->work->shown, 0, 1, 2);
}
