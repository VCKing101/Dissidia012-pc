typedef struct MenuSaveConfig { char pad_00[0x2C]; int field_2C; int field_30; } MenuSaveConfig;
typedef struct MenuSaveView { char pad_00[0x48]; int field_48; } MenuSaveView;
extern MenuSaveConfig *D_08A4FFF4;
void func_08A4C5E0(MenuSaveView *self, int value) { if (self->field_48 == value) { return; } self->field_48 = value; D_08A4FFF4->field_2C = value; D_08A4FFF4->field_30 = value; }
