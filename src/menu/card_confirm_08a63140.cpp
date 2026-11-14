/* Confirms the current card; when it accepts, closes the cards and asks the follow-up question. */
class ConfirmCard {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual int v12(int mode);
};

struct ConfirmCards {
    char pad_000[0xD14];
    ConfirmCard *field_D14[7];
    char pad_D30[0xD90 - 0xD30];
    int field_D90;
};

extern "C" void *D_08A4FFF4;
extern "C" char D_08A61EA0[];
extern "C" int func_08A62F88(ConfirmCards *self);
extern "C" void func_08A621C8(ConfirmCards *self);
extern "C" void func_08A61EC0(void);
extern "C" void func_08A4E2E4(void *dialog, int text, void *onYes, void *onNo, void *onCancel,
                              int sound, int a, int b, int c);

extern "C" {
bool func_08A63140(ConfirmCards *self)
{
    int current = self->field_D90;
    int mode = func_08A62F88(self);
    if (self->field_D14[current]->v12(mode) == 0) {
        return false;
    }
    func_08A621C8(self);
    func_08A4E2E4(D_08A4FFF4, 0, (void *)func_08A61EC0, D_08A61EA0, D_08A61EA0, 0x62, -1, -1, -1);
    return true;
}
}
