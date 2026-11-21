#include "menu/agent36_types.h"

/* Battle setup: opens the three-entry cursor at first and plays the mark
 * motion (0x24/0x25 by the text count; 0x20/0x21 then state 0x24/0x25
 * when not shown yet). */

extern "C" ScriptCmd36 D_08A4547A[];

struct CursorShow36 {
    static void Set(Cursor36 *cursor, unsigned int flag) { cursor->flags |= flag; }
};

extern "C" void func_08A422F8(BattleGlobal36 *self, int first, unsigned char shown)
{
    self->table->field_129A = 0;
    func_08A4146C(self, 1);
    func_088D46E4(&self->table->cursor, first, 0, 2, 0, 3);
    CursorShow36::Set(&self->table->cursor, 4);
    if (self->table->text_count == 6) {
        if (shown) {
            func_088D9F5C(self->table->handles[0x13], 0x24, 1);
        } else {
            func_088D9F5C(self->table->handles[0x13], 0x20, 1);
            ((Model36 *)self->table->handles[0x13])->field_0E = 0x24;
        }
    } else {
        if (shown) {
            func_088D9F5C(self->table->handles[0x13], 0x25, 1);
        } else {
            func_088D9F5C(self->table->handles[0x13], 0x21, 1);
            ((Model36 *)self->table->handles[0x13])->field_0E = 0x25;
        }
    }
    func_088D3CF0(self->table->layout, self->table->handles, D_08A4547A);
    func_088D9CFC(self->table->handles[0x12]);
}
