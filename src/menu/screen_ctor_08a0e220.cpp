struct ScreenBodyA {
    char pad_000[0xFCC];
    int field_FCC;
    int field_FD0;
    char pad_FD4[8];
    int field_FDC;
    int field_FE0;
    char pad_FE4[0x1310 - 0xFE4];
};

struct ScreenA {
    void *field_00;
    char pad_04[0xC];
    ScreenBodyA field_10;
    ScreenBodyA *field_1320;

    static ScreenA *init(ScreenA *self, int param, void *vtable)
    {
        self->field_00 = vtable;
        self->field_1320 = &self->field_10;
        self->field_1320->field_FCC = param;
        self->field_1320->field_FD0 = 0;
        self->field_1320->field_FDC = 0;
        self->field_1320->field_FE0 = 0;
        return self;
    }
};

extern "C" {

ScreenA *func_08A24048(ScreenA *self, int param);
extern int D_08A11510;

ScreenA *func_08A0E220(ScreenA *self, int param)
{
    func_08A24048(self, param);
    return ScreenA::init(self, param, &D_08A11510);
}

}
