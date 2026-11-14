/* Membership test on the halfword at +0x12 of the record held at +0x20. The
   switch's jump table is .rodata at 0x089CE8F0, built from this unit and
   counted as data. */

typedef struct KindRecord {
    char pad[0x12];
    short kind;
} KindRecord;

typedef struct KindOwner {
    char pad[0x20];
    KindRecord *record;
} KindOwner;

int func_089237E8(KindOwner *self)
{
    switch (self->record->kind) {
    case 3: case 4: case 5: case 6: case 9: case 13:
        return 1;
    default:
        return 0;
    }
}
