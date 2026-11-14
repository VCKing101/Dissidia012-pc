typedef struct MenuPanel {
    void *vtable;
    char pad_004[0x738];
    char field_73C[0x2C];
    unsigned char field_768;
} MenuPanel;

extern char D_08A0BEB0[];
extern void func_0891A810(MenuPanel *self);
extern void func_0891A254(void *part);

MenuPanel *func_08A0BA30(MenuPanel *self) {
    func_0891A810(self);
    self->vtable = D_08A0BEB0;
    self->field_768 = 0;
    func_0891A254(self->field_73C);
    return self;
}
