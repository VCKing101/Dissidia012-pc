struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x2C];
    int field_2C;
    char pad_30[0xE0 - 0x30];
    MarkGauge *field_E0;
};

extern "C" void func_088D9F5C(MarkGauge *target, int kind, int value);

extern "C" {
void func_08A57B6C(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_E0)) {
        return;
    }
    func_088D9F5C(self->field_E0, 1, 1);
    self->field_2C = 2;
}
}
