typedef struct MenuRecord {
    char pad_000[0xD4];
    int field_D4;
    char pad_0D8[0x24C - 0xD8];
} MenuRecord;

typedef struct MenuRecordSet {
    char pad_0000[0x8];
    MenuRecord field_08[1];
} MenuRecordSet;

typedef struct MenuGlobalState {
    char pad_0000[0x2BB8];
    MenuRecordSet field_2BB8;
} MenuGlobalState;

typedef struct MenuGlobal {
    char pad_000[0x154];
    MenuGlobalState *field_154;
} MenuGlobal;

typedef struct MenuRecordView {
    char pad_000[0xC7C];
    int field_C7C;
} MenuRecordView;

typedef struct MenuRecordOwner {
    char pad_000[0xD80];
    MenuRecordView *field_D80;
} MenuRecordOwner;

extern MenuGlobal *D_08A23FF0;

int func_08A172F0(MenuRecordOwner *self) {
    int result = 0;
    MenuRecord *record = &D_08A23FF0->field_154->field_2BB8.field_08[self->field_D80->field_C7C];
    if (record->field_D4 == 1) {
        result = 1;
    }
    return result;
}
