struct MarkGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const MarkGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct MarkPanel {
    char pad_00[0x10];
    int field_10;
    char pad_14[8];
    MarkGauge *field_1C;
    char pad_20[0x74];
    int field_94;
};

extern "C" int D_08A4FFF4;
extern "C" void func_08A4E830(int handle, int value);

extern "C" {
void func_08A650CC(MarkPanel *self)
{
    if (!MarkGauge::isMarked(self->field_1C)) {
        return;
    }
    func_08A4E830(D_08A4FFF4, self->field_94);
    self->field_10 = 2;
}
}
