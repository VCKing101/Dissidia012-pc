/* Returns the table position of an event's id for kind 1 (or kind 2 with flag 0x1000). */
typedef struct EventInfo {
    unsigned char field_00;
    signed char field_01;
    char pad_02[0x2];
    unsigned int field_04;
} EventInfo;

typedef struct EventRow {
    unsigned short field_00;
    char pad_02[0xC];
} EventRow;

extern EventRow D_08A0AF92[];

signed char func_089EF420(void *self, EventInfo *info)
{
    unsigned short result = 0;
    if (info->field_00 == 1 || (info->field_00 == 2 && (info->field_04 & 0x1000) != 0)) {
        int i = 0;
        EventRow *row = D_08A0AF92;
        for (;;) {
            if (row->field_00 == 0xFFFF) {
                break;
            }
            if (row->field_00 == info->field_01) {
                result = i;
                break;
            }
            row++;
            i++;
        }
    }
    return result;
}
