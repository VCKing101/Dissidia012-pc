#include "menu/agent36_types.h"

/* Name list screen: whether record counts for kind: always when
 * func_08A430B8 says so, never without a value; some kinds also need
 * value 0x26, others a value other than -1, the rest the character to be
 * listed (func_08A3D748) and a first value. */

extern "C" {
int func_08A430B8(void);
int func_08A3D748(NameScreen36 *self, int chara);
}

extern "C" bool func_08A3DD10(NameScreen36 *self, NameRecord36 *record, int kind)
{
    if (func_08A430B8() != 0) {
        return true;
    }
    int *values = record->values;
    if (values[kind] == 0) {
        return false;
    }
    switch (kind) {
    case 3:
    case 4:
    case 6:
    case 32:
    case 33:
        if (values[0x26] == 0) {
            break;
        }
        return true;
    case 34:
    case 35:
    case 36:
        if (values[kind] == -1) {
            break;
        }
        return true;
    default:
        if (func_08A3D748(self, record->chara) != 0) {
            if (kind == 8) {
                return true;
            }
            if (values[0] != 0) {
                return true;
            }
        }
        break;
    }
    return false;
}
