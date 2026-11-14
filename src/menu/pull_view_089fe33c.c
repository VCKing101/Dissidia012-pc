#include "menu/agent29_types.h"

/* Screen0B6A0: moves the pull list's view to the record under the cursor
   (+0x30378 of the block at +0x38A0 of the work): scrolls to its place
   (11 units a step), shows its entry and keeps its key (+0x303E8). */
typedef struct PullRecord29 {
    unsigned char flags;
    char pad_01[0x1];
    unsigned short entry;
    short major;
    short minor;
} PullRecord29;

typedef struct PullBlock29 {
    int kind;
    char pad_04[0x26D68];
    PullRecord29 records[0xC80]; /* +0x26D6C */
    int count;                   /* +0x2D16C */
    char pad_2D170[0x3208];
    int cursor;                  /* +0x30378 */
    char pad_3037C[0x6C];
    short shown_major;           /* +0x303E8 */
    short shown_minor;           /* +0x303EA */
} PullBlock29;

extern void func_089FD1F0(Screen0B6A0 *self, int arg, short x, short y);
extern void func_089FD258(Screen0B6A0 *self, short entry);

void func_089FE33C(Screen0B6A0 *self)
{
    PullBlock29 *block = (PullBlock29 *)&self->work->field_38A0;
    short x = block->records[block->cursor].major * 11;
    short y = block->records[block->cursor].minor * 11;

    func_089FD1F0(self, 1, x, y);
    func_089FD258(self, block->records[block->cursor].entry);
    block->shown_major = block->records[block->cursor].major;
    block->shown_minor = block->records[block->cursor].minor;
}
