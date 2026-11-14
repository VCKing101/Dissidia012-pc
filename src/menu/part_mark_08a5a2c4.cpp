struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x2C];
    int field_2C;
    char pad_30[0x44 - 0x30];
    MarkGauge *field_44;
};

extern "C" void func_088D9F5C(MarkGauge *target, int kind, int value);

extern "C" {
void func_08A5A2C4(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_44)) {
        return;
    }
    func_088D9F5C(self->field_44, 1, 1);
    self->field_2C = 2;
}
}
