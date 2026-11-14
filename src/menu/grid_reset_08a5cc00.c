typedef struct GridCell {
    char data[0x54];
} GridCell;

typedef struct GridPanel {
    char pad_000[0x28];
    int field_28;
    int field_2C;
    int field_30[3];
    int field_3C[4];
    int field_4C[1];
    char pad_050[0xA8 - 0x50];
    GridCell field_A8[16];
    char pad_5E8[0x644 - 0x5E8];
    int field_644;
} GridPanel;

extern void func_08A51E3C(GridPanel *self);
extern void func_08A5C5F0(GridCell *cell);

void func_08A5CC00(GridPanel *self)
{
    int i;
    func_08A51E3C(self);
    self->field_28 = 0;
    for (i = 0; i < 3; i++) {
        self->field_30[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        self->field_3C[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->field_4C[i] = 0;
    }
    self->field_2C = 0;
    for (i = 0; i < 16; i++) {
        func_08A5C5F0(&self->field_A8[i]);
    }
    self->field_644 = 0;
}
