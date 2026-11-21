#include "menu/agent36_types.h"

typedef struct Entry484D8 {
    unsigned short field_00;  /* +0x00: column 2 */
    char pad_02[0x2];
    unsigned short field_04;  /* +0x04: column 6 */
    char pad_06[0x2];
    unsigned char field_08;   /* +0x08 */
    unsigned char field_09;   /* +0x09 */
    unsigned short field_0A;  /* +0x0A: column 1 */
    unsigned short field_0C;  /* +0x0C: column 3 */
    unsigned short field_0E;  /* +0x0E */
    unsigned short field_10;  /* +0x10: column 4 */
    unsigned short field_12;  /* +0x12 */
    unsigned short field_14;  /* +0x14: column 5 */
} Entry484D8;

typedef struct Table484D8 {
    char pad_00[0x8];
    unsigned short *ids;      /* +0x08 */
} Table484D8;

Entry484D8 *func_08A3C2B8(unsigned short id);
int func_08A48610(Table484D8 *self, unsigned short id);

int func_08A484D8(Table484D8 *self, int column, int row)
{
    int result = -1;
    Entry484D8 *entry = func_08A3C2B8(self->ids[row]);

    if (column == 0) {
        result = 0;
        if (entry->field_08 == 1) {
            result = 1;
        }
        if (entry->field_09 == 1) {
            result |= 2;
        }
        if (func_08A48610(self, entry->field_0E) >= 0) {
            result |= 4;
        }
        if (func_08A48610(self, entry->field_12) == 1) {
            result |= 8;
        }
    } else if (column == 1) {
        result = entry->field_0A;
    } else if (column == 2) {
        result = entry->field_00;
    } else if (column == 3) {
        result = entry->field_0C;
    } else if (column == 4) {
        result = entry->field_10;
    } else if (column == 5) {
        result = entry->field_14;
    } else if (column == 6) {
        result = entry->field_04;
    }
    return result;
}
