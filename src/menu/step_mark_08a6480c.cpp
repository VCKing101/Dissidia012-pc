struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x10];
    int field_10;
    char pad_14[4];
    MarkGauge *field_18;
};

extern "C" void func_08A64AAC(MarkPanel *self);

extern "C" {
void func_08A6480C(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_18)) {
        return;
    }
    func_08A64AAC(self);
    self->field_10 = 1;
}
}
