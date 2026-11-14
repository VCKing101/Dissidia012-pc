extern "C" {
extern void func_08A03944(void *);
extern void func_08A10B88(void *);
extern void func_08A06264(void *);
extern void func_089F5808(void *);
extern void func_089F0F58(void *);
extern void func_08A11350(void *);
extern void func_08A155DC(void *);
extern void func_08A15A04(void *);
extern void func_08A06D0C(void *);
}

struct Part34 { char data[0x34]; };
struct Part2C { char data[0x2C]; };
struct PartB0 { char data[0xB0]; };

struct Unit6D38 {
    char pad_000[0x20];
    char field_20[0x8];
    char field_28[0xC];
    char field_34[0x14];
    char field_48[0x14];
    char field_5C[0x108];
    Part34 field_164[2];
    Part2C field_1CC[2];
    PartB0 field_224[2];
};

extern "C" {
void func_08A06D38(Unit6D38 *self)
{
    for (int i = 0; i < 2; i++) {
        func_08A03944(&self->field_224[i]);
        func_08A10B88(&self->field_1CC[i]);
        func_08A06264(&self->field_164[i]);
    }
    func_089F5808(self->field_5C);
    func_089F0F58(self->field_48);
    func_08A11350(self->field_34);
    func_08A155DC(self->field_28);
    func_08A15A04(self->field_20);
    func_08A06D0C(self);
}
}
