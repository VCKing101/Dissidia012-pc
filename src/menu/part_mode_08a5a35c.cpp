struct ModeGauge {
    char pad_00[0x20];
    unsigned short field_20;

    static bool isMarked(const ModeGauge *gauge) { return (gauge->field_20 & 0x20) != 0; }
};

struct ModePanel {
    char pad_00[0x30];
    int field_30;
    char pad_34[0x10];
    ModeGauge *field_44;
};

extern "C" int D_08A4FFF4;
extern "C" void func_08A4E478(int handle);
extern "C" void func_08A4E4C0(int handle);
extern "C" void func_08A5A02C(ModePanel *self);

extern "C" {
void func_08A5A35C(ModePanel *self)
{
    if (!ModeGauge::isMarked(self->field_44)) {
        return;
    }
    switch (self->field_30) {
    case 0:
        func_08A4E478(D_08A4FFF4);
        break;
    case 1:
        func_08A4E4C0(D_08A4FFF4);
        break;
    }
    func_08A5A02C(self);
}
}
