#include "menu/agent29_types.h"

/* Screen0B6A0: takes the entry under the cursor (+0x3374, 1-based) as the
   current one (+0x30D4), and for a set (kind 1, flag 0x800) opens the set
   cursor (+0x33E4) over its rows; with no entry selected, either opens
   the kind-0x16 help or reshows the page. */
struct PickWork29 {
    char pad_0000[0x29CC];
    Entry84 entries[9];        /* +0x29CC */
    char pad_2E70[0x261];
    unsigned char reshow;      /* +0x30D1 */
    char pad_30D2[0x2];
    Entry84 current;           /* +0x30D4 */
    char pad_3158[0x21C];
    int cursor_index;          /* +0x3374 */
    char pad_3378[0x6C];
    MenuCursor set_cursor;     /* +0x33E4 */
    unsigned char set_open;    /* +0x3410 */
};

struct PickScreen29 {
    char pad_000[0x14C];
    PickWork29 *work;
};

extern "C" {
void func_089F4544(PickScreen29 *self);
int func_089F4394(PickScreen29 *self, Entry84 *entry);
void func_089F4E28(PickScreen29 *self, int arg);
void func_089F41C8(PickScreen29 *self, int arg);

void func_089F43D8(PickScreen29 *self)
{
    self->work->set_open = 0;
    if (self->work->cursor_index > 0) {
        self->work->current = self->work->entries[self->work->cursor_index - 1];
        func_089F4544(self);
        self->work->reshow = 0;
        if (self->work->current.kind == 1 && (self->work->current.flags & 0x800) != 0) {
            int rows = func_089F4394(self, &self->work->current);
            if (rows != 0) {
                func_088D46E4(&self->work->set_cursor, 0, 0, rows, 0, -1);
                MenuCursor *cursor = &self->work->set_cursor;
                cursor->flags |= 2;
                self->work->set_open = 1;
            }
        }
    } else if (func_089EFB50((Screen0B6A0 *)self, 0x16, -1) == 0) {
        func_089F4E28(self, 1);
    } else {
        func_089F4544(self);
        self->work->reshow = 1;
    }
    func_089F41C8(self, 0);
}
}
