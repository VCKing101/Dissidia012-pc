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
    char pad_2C[0x10];
    MarkGauge *field_3C;
};

extern "C" void func_08A5CF38(MarkPanel *self);

extern "C" {
void func_08A5D184(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_3C)) {
        return;
    }
    func_08A5CF38(self);
    self->field_18 = 0;
    self->field_28 = 5;
}
}
