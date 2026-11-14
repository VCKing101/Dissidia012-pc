extern "C" {
extern void func_089FC71C(void *);
extern void func_089FDEF8(void *);
extern void func_08A1681C(void *);
extern void func_08A01844(void *);
extern void func_088A1E44(void *handle, float fade);
}

class Unit08A27BB4Base {
public:
    virtual void slot_008();
    virtual void slot_010();
};

class Unit08A27BB4 : public Unit08A27BB4Base {
public:
    char pad_004[0x10];
    void *field_14;
    char pad_018[0x4];
    char field_1C[0x28];
    char field_44[0x30];
    char field_74[0x34];
    char field_A8[0x4];
};

extern "C" {
void func_08A27BB4(Unit08A27BB4 *self)
{
    func_089FC71C(self->field_A8);
    func_089FDEF8(self->field_74);
    func_08A1681C(self->field_44);
    func_08A01844(self->field_1C);
    if (self->field_14) {
        func_088A1E44(self->field_14, 0.0f);
        self->field_14 = 0;
    }
    self->slot_010();
}
}
