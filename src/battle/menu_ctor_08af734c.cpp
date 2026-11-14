typedef struct MenuSelection {
    int field_24;
    int field_28;
    int field_2C;

    static void init(MenuSelection *selection)
    {
        selection->field_24 = 0;
        selection->field_28 = -1;
        selection->field_2C = -1;
    }
} MenuSelection;

typedef struct MenuPanel {
    char pad_00[0xC];
    char field_0C[0x18];
    MenuSelection selection;
    char pad_30[0xC];
    int field_3C;
    int field_40;
    unsigned char field_44;
    unsigned char field_45;
} MenuPanel;

extern "C" {

extern void func_08AF3B90(MenuPanel *self);
extern void func_08935828(void *dest, int value, int size);

MenuPanel *func_08AF734C(MenuPanel *self)
{
    func_08AF3B90(self);
    MenuSelection::init(&self->selection);
    self->field_3C = 0;
    self->field_40 = 0;
    self->field_44 = 0;
    self->field_45 = 0;
    func_08935828(self->field_0C, 0, 0x18);
    return self;
}

}
