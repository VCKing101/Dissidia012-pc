struct ScreenBodyC {
    char pad_000[0x10E8];
    int field_10E8;
    char pad_10EC[4];
    int field_10F0;
    int field_10F4;
    int field_10F8;
    char pad_10FC[0x17A0 - 0x10FC];
};

struct ScreenC {
    void *field_00;
    char pad_04[0xC];
    ScreenBodyC field_10;
    ScreenBodyC *field_17B0;

    static ScreenC *init(ScreenC *self, void *vtable)
    {
        self->field_00 = vtable;
        self->field_17B0 = &self->field_10;
        self->field_17B0->field_10E8 = 0;
        self->field_17B0->field_10F0 = 0;
        self->field_17B0->field_10F4 = 0;
        self->field_17B0->field_10F8 = 0;
        return self;
    }
};

extern "C" {

ScreenC *func_08A24048(ScreenC *self);
extern int D_08A1AB08;

ScreenC *func_08A19638(ScreenC *self)
{
    func_08A24048(self);
    return ScreenC::init(self, &D_08A1AB08);
}

}
