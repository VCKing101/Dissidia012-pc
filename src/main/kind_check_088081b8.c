/* Returns 1 when the record's kind is one of 4, 5, 6, 9 or 12 to 15. */
typedef struct Record088081B8 {
    char pad_00[4];
    unsigned char kind;
} Record088081B8;

typedef struct Owner088081B8 {
    char pad_00[0x34];
    Record088081B8 *record;
} Owner088081B8;

unsigned char func_088081B8(Owner088081B8 *self)
{
    int result = 0;
    unsigned char kind = self->record->kind;
    if (kind == 4 || kind == 6 || kind == 5 || kind == 9 || kind == 12 || kind == 13 || kind == 14 || kind == 15) {
        result = 1;
    }
    return result;
}
