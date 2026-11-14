#include "menu/agent29_types.h"

/* MenuRoot: an error from the network: keeps it (+0x25B80) and, unless
   the save has finished, shows the error dialog for it (func_088B1194
   with the callback func_08A2044C); when that fails, flag 0x80, else the
   code shown (+0x25B84); then step 0xD. The two known codes have no
   case of their own any more. */

void func_08A1CA60(MenuRoot *self, unsigned int error)
{
    WORK_INT(self->work, 0x25B80) = error;
    switch (error) {
    case 0x80410B03:
        break;
    case 0x80410D0C:
        break;
    }
    if ((WORK_INT(self->work, 0x8E60) & 0x20) == 0) {
        if (func_088B1194(D_089D727C, error, func_08A2044C, 0, 0xA0) != 0) {
            WORK_INT(self->work, 0x8E60) |= 0x80;
        } else {
            WORK_INT(self->work, 0x25B84) = error;
        }
        WORK_INT(self->work, 0x6968) = 0xD;
    }
}
