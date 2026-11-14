/* Loads the model for id (none for -1) after resetting the panel (virtual slot 10), then binds it. */
class ModelPanelBase {
    char data[0x24];
public:
    virtual void slot_08();
    virtual void slot_10();
    virtual void slot_18();
    virtual void slot_20();
    virtual void slot_28();
    virtual void slot_30();
    virtual void slot_38();
    virtual void slot_40();
    virtual void slot_48();
    virtual void slot_50();
};

class ModelPanel : public ModelPanelBase {
public:
    char pad_28[0x10];
    char field_38[0x8];
    void *field_40;
    void *field_44;
};

extern "C" {
void *func_0887B3EC(int bank, int group, int a, int id, int b, int c);
void *func_0887E534(void *model);
void func_088D428C(char *slot, void **model, void (*draw)(void), int show);
void func_08A606D0(void);

void func_08A60310(ModelPanel *self, int bank, int group, int id)
{
    if (id == -1) {
        return;
    }
    self->slot_50();
    self->field_40 = func_0887B3EC(bank, group, 0, id, 0, 0);
    self->field_44 = func_0887E534(self->field_40);
    func_088D428C(self->field_38, &self->field_40, func_08A606D0, 1);
}
}
