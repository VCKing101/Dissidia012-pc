#include "menu/agent29_types.h"

/* MenuRoot: raises flag 0x200 (+0x8E60); with flags 8 and 0x10 also set
   it is handed to the lobby object in D_089D8364 (+0x16E5), or dropped
   again when there is none. */
typedef struct Lobby29 {
    char pad_0000[0x16E5];
    unsigned char notice;
} Lobby29;

extern Lobby29 *D_089D8364;

void func_08A1DDE8(MenuRoot *self)
{
    unsigned int flags;

    WORK_INT(self->work, 0x8E60) |= 0x200;
    flags = WORK_INT(self->work, 0x8E60);
    if ((flags & 8) != 0) {
        if ((flags & 0x10) != 0) {
            if (D_089D8364 != 0) {
                D_089D8364->notice = 1;
            } else {
                WORK_INT(self->work, 0x8E60) = flags & ~0x200;
            }
        }
    }
}
