/* Resets the panel (virtual slot 9), allocates its four sprites and builds their text. */
class SpritePanelBase {
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
};

class SpritePanel : public SpritePanelBase {
public:
    char pad_28[0x8];
    void *field_30;
    void *field_34;
    void *field_38;
    int field_3C[2];
    int field_44;
    int field_48;
};

extern "C" {
int func_088DAA68(void *bank);
void func_088DAB7C(void *bank, int sprite);
void func_088D3CF0(int layer, int *sprites, void (*draw)(void));
void func_08A5DC00(void);

void func_08A5D460(SpritePanel *self)
{
    int i;
    self->slot_48();
    for (i = 0; i < 2; i++) {
        self->field_3C[i] = func_088DAA68(self->field_34);
        func_088DAB7C(self->field_34, self->field_3C[i]);
    }
    self->field_44 = func_088DAA68(self->field_38);
    self->field_48 = func_088DAA68(self->field_30);
    func_088D3CF0(0, self->field_3C, func_08A5DC00);
}
}
