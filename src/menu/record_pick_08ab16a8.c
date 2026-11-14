/* Copies the current page's selected record (or a default) into the work record. */
typedef struct PickRecord {
    int field_00[0x154 / 4];
} PickRecord;

typedef struct PickPages {
    char pad_0000[0x5];
    signed char field_05;
    char pad_0006[0x151 - 0x6];
    signed char field_151[(0x194 - 0x151)];
    PickRecord field_194[15];
} PickPages;

typedef struct PickWork {
    char pad_0000[0x1586];
    signed char field_1586;
    char pad_1587;
    PickRecord field_1588;
} PickWork;

typedef union PickState {
    PickPages pages;
    PickWork work;
} PickState;

extern void func_0892A13C(PickRecord *record);

int func_08AB16A8(PickState *self)
{
    PickRecord *record = &self->work.field_1588;
    int index = self->pages.field_151[self->pages.field_05] - 1;
    self->work.field_1586 = self->pages.field_05;
    if (index >= 0) {
        *record = self->pages.field_194[index];
    } else {
        func_0892A13C(record);
    }
    return index;
}
