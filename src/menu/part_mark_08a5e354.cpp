struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x28];
    int field_28;
    char pad_2C[0x14];
    MarkGauge *field_40;
};

extern "C" void func_088D9F5C(MarkGauge *target, int kind, int value);

extern "C" {
void func_08A5E354(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_40)) {
        return;
    }
    func_088D9F5C(self->field_40, 1, 1);
    self->field_28 = 2;
}
}
