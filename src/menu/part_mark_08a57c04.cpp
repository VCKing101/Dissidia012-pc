struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x18];
    int field_18;
    char pad_1C[0x10];
    int field_2C;
    char pad_30[0xE0 - 0x30];
    MarkGauge *field_E0;
};

extern "C" void func_08A576B4(MarkPanel *self);

extern "C" {
void func_08A57C04(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_E0)) {
        return;
    }
    func_08A576B4(self);
    self->field_18 = 0;
    self->field_2C = 5;
}
}
