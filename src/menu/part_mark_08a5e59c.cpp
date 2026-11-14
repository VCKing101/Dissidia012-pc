struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x18];
    int field_18;
    char pad_1C[0xC];
    int field_28;
    char pad_2C[0x14];
    MarkGauge *field_40;
};

extern "C" void func_08A5E0E4(MarkPanel *self);

extern "C" {
void func_08A5E59C(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_40)) {
        return;
    }
    func_08A5E0E4(self);
    self->field_18 = 0;
    self->field_28 = 5;
}
}
