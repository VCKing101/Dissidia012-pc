#include "menu/agent29_types.h"

/* Screen0B6A0: the pull's record list, kept in a block that starts at
   +0x38A0 of the work: finds the place of a (major, minor) key among the
   records whose flags hold `mask`, and the next or previous record from
   a key. */
typedef struct PullRecord29 {
    unsigned char flags;
    char pad_01[0x3];
    short major;
    short minor;
} PullRecord29;

typedef struct PullBlock29 {
    int kind;
    char pad_04[0x26D68];
    PullRecord29 records[0xC80];
    int count; /* +0x2D16C */
} PullBlock29;

int func_089FDDA4(Screen0B6A0 *self, short major, short minor, unsigned char mask)
{
    PullBlock29 *block = (PullBlock29 *)&self->work->field_38A0;
    int result = -1;
    int i;

    for (i = 0; i < block->count; i++) {
        if ((block->records[i].flags & mask) == mask) {
            if (major < block->records[i].major) {
                result = i;
                break;
            }
            if (major == block->records[i].major && minor < block->records[i].minor) {
                result = i;
                break;
            }
        }
    }
    if (i == block->count) {
        result = block->count;
    }
    return result;
}

