struct LockGauge {
    char pad_00[0x20];
    unsigned short field_20;
};

struct LockPanel {
    char pad_00[0x14];
    LockGauge *field_14[2];
};

extern "C" void func_088D9D24(LockGauge *gauge);

extern "C" {
void func_08A646C4(LockPanel *self)
{
    for (int i = 0; i < 2; i++) {
        LockGauge *gauge = self->field_14[i];
        gauge->field_20 = gauge->field_20 & ~0x4000;
        func_088D9D24(gauge);
    }
}
}
