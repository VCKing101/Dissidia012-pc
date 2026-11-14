#include "menu/agent29_types.h"

/* Screen0B6A0: shows or hides the four scroll arrows of the pull list
   (second handle set 10/11 for the page, 8/9 for the line), hiding each
   one at its end. */
typedef struct PullScroll29 {
    char pad_000[0x112];
    short page;      /* +0x112 */
    short line;      /* +0x114 */
    short last_page; /* +0x116 */
    short last_line; /* +0x118 */
} PullScroll29;

void func_089FE180(Screen0B6A0 *self)
{
    PullScroll29 *scroll = (PullScroll29 *)&self->work->field_38A0;

    if (scroll->page == 0) {
        func_088D9CFC(self->work->handles2[10]);
    } else {
        func_088D9CD8(self->work->handles2[10]);
    }
    if (scroll->page == scroll->last_page) {
        func_088D9CFC(self->work->handles2[11]);
    } else {
        func_088D9CD8(self->work->handles2[11]);
    }
    if (scroll->line == 0) {
        func_088D9CFC(self->work->handles2[8]);
    } else {
        func_088D9CD8(self->work->handles2[8]);
    }
    if (scroll->line == scroll->last_line) {
        func_088D9CFC(self->work->handles2[9]);
    } else {
        func_088D9CD8(self->work->handles2[9]);
    }
}
