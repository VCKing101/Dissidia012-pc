typedef struct ScenePanel {
    const void *vtable;
    char field_04[0x14];
    char field_18[0xC];
    int field_24;
    int field_28;
    unsigned char field_2C;
    unsigned char field_2D;
    char pad_2E[0x2];
    int field_30;
    char field_34[0x8];
    int field_3C;
} ScenePanel;

extern const char D_08AFC570[];
extern void func_088EC4C4(void *sub);
extern void func_088EC278(void *sub);
extern void func_08935828(void *dest, int value, int size);

ScenePanel *func_08AFBDD8(ScenePanel *self)
{
    self->vtable = D_08AFC570;
    func_088EC4C4(self->field_04);
    func_088EC278(self->field_18);
    self->field_24 = 0;
    self->field_28 = 0;
    self->field_2C = 0;
    self->field_2D = 0;
    self->field_30 = 0;
    self->field_3C = 0;
    func_08935828(self->field_34, 0, 8);
    return self;
}
