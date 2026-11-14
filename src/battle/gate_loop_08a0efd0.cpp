extern "C" {
extern void func_08A0F1B4(void *);
extern void func_08A0F60C(void *);
extern void func_089F83AC(void *);
extern void func_08A08334(void *);
}

struct GateEntry38 { char data[0x38]; };
struct GateEntry34 { char data[0x34]; };
struct GateEFD0 {
    unsigned char field_00;
    char pad_001[0x33];
    GateEntry38 field_34[2];
    GateEntry34 field_A4[2];
};

extern "C" {
int func_08A0EFD0(GateEFD0 *self)
{
    if (!self->field_00) {
        return -1;
    }
    func_08A0F1B4(self);
    func_08A0F60C(self);
    for (int i = 0; i < 2; i++) {
        func_089F83AC(&self->field_34[i]);
        func_08A08334(&self->field_A4[i]);
    }
    return 0;
}
}
