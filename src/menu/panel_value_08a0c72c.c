typedef struct MenuRecord {
    unsigned short field_00;
    unsigned short field_02;
    char pad_04[0x248];
} MenuRecord;

typedef struct MenuRecordSet {
    char pad_0000[0x8];
    MenuRecord field_08[1];
    char pad_0254[0x3BC4 - 0x254];
    short field_3BC4[1];
} MenuRecordSet;

typedef struct MenuGlobalState {
    char pad_0000[0x2BB8];
    MenuRecordSet field_2BB8;
} MenuGlobalState;

typedef struct MenuGlobal {
    char pad_000[0x154];
    MenuGlobalState *field_154;
} MenuGlobal;

typedef struct MenuPanelState {
    char pad_0000[0xFEC];
    int field_FEC;
    char pad_0FF0[0x1070 - 0xFF0];
    int field_1070;
} MenuPanelState;

typedef struct MenuPanelOwner {
    char pad_0000[0x1320];
    MenuPanelState *field_1320;
} MenuPanelOwner;

extern MenuGlobal *D_08A23FF0;

void func_08A0C72C(MenuPanelOwner *self) {
    MenuPanelState *state = self->field_1320;
    MenuRecordSet *set = &D_08A23FF0->field_154->field_2BB8;
    MenuRecord *record = &set->field_08[set->field_3BC4[state->field_1070]];
    state->field_FEC = record->field_02;
}
