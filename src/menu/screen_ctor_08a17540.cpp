struct ScreenBodyB {
    char pad_000[0xC7C];
    int field_C7C;
    int field_C80;
    char pad_C84[4];
    int field_C88;
    int field_C8C;
    char pad_C90[0xD70 - 0xC90];
};

struct ScreenB {
    void *field_00;
    char pad_04[0xC];
    ScreenBodyB field_10;
    ScreenBodyB *field_D80;

    static ScreenB *init(ScreenB *self, int param, void *vtable)
    {
        self->field_00 = vtable;
        self->field_D80 = &self->field_10;
        self->field_D80->field_C7C = param;
        self->field_D80->field_C80 = 0;
        self->field_D80->field_C88 = 0;
        self->field_D80->field_C8C = 0;
        return self;
    }
};

extern "C" {

ScreenB *func_08A24048(ScreenB *self, int param);
extern int D_08A181E0;

ScreenB *func_08A17540(ScreenB *self, int param)
{
    func_08A24048(self, param);
    return ScreenB::init(self, param, &D_08A181E0);
}

}
