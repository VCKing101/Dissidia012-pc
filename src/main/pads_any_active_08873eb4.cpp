struct Pad { int field_00; int field_04; int field_08; float value; char data_10[0x234 - 0x10]; int pressed; char data_238[8]; };
extern "C" {
extern Pad D_09B47B50[3];
int func_08873EB4(void)
{
    for (int i = 0; i < 3; i++) {
        if (D_09B47B50[i].pressed != 0 || D_09B47B50[i].value != 0.0f) {
            return 1;
        }
    }
    return 0;
}
}
