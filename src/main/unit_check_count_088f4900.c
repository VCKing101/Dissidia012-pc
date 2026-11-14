/* Counts the matches of a check; for kind 0x19 drops the entries whose table id is unavailable. */
typedef struct CheckRecord {
    unsigned char field_00;
    unsigned char kind;
} CheckRecord;

extern int D_089CCCF8[];
extern int func_088C51AC(int check, void *list, CheckRecord *record, unsigned char flag);
extern int func_088678A8(int id);
extern void func_088F4894(void *list, int index, int count);

int func_088F4900(int check, void *list, CheckRecord *record, unsigned char flag)
{
    int count = func_088C51AC(check, list, record, flag);

    if (record->kind == 0x19) {
        int kept = 0;
        int i;

        for (i = 0; i < count; i++) {
            if (func_088678A8(D_089CCCF8[i]) == 0) {
                func_088F4894(list, kept, count);
            } else {
                kept++;
            }
        }
        count = kept;
    }
    return count;
}
