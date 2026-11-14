struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x28];
    int field_28;
    char pad_2C[0x10];
    MarkGauge *field_3C;
};

extern "C" void func_088D9F5C(MarkGauge *target, int kind, int value);

extern "C" {
void func_08A5F4C8(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_3C)) {
        return;
    }
    func_088D9F5C(self->field_3C, 2, 1);
    self->field_28 = 2;
}
}
