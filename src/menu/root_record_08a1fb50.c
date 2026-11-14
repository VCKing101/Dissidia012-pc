typedef struct MenuRecord {
    int field_00;
    char field_04[0x248];
} MenuRecord;

typedef struct MenuRecordSet {
    char pad_0000[0x8];
    MenuRecord field_08[1];
    char pad_0254[0x3BC4 - 0x254];
    short field_3BC4[1];
} MenuRecordSet;

typedef struct MenuRootState {
    char pad_0000[0x2BB8];
    MenuRecordSet field_2BB8;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

extern void func_088F5EA0(void *target, char *source);

void func_08A1FB50(MenuRootOwner *self, int index, void *target) {
    MenuRecordSet *set = &self->field_154->field_2BB8;
    MenuRecord *record = &set->field_08[set->field_3BC4[index]];
    func_088F5EA0(target, record->field_04);
}
