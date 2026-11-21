#include "menu/agent36_types.h"

/* Whether every row of table is known (one checker per table; table 1 needs
 * both of its checks). */

extern "C" {
bool func_08A3C548(void);
bool func_08A3C5E8(void);
bool func_08A3C868(void);
bool func_08A3C90C(void);
bool func_08A3C9C8(void);
bool func_08A3CB5C(void);
}

extern "C" bool func_08A4175C(void *self, int table)
{
    switch (table) {
    case 2:
        return func_08A3C548();
    case 3:
        return func_08A3C5E8();
    case 0:
        return func_08A3C90C();
    case 1:
        if (!func_08A3C868()) {
            return false;
        }
        if (!func_08A3C9C8()) {
            return false;
        }
        return true;
    case 4:
        return func_08A3CB5C();
    }
    return false;
}
