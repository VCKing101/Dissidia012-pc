typedef struct MenuPage {
    void *effects[3];
    char pad_0C[0x8];
    void *window;
    char pad_18[0x800];
    int field_818;
} MenuPage;

extern "C" {

extern void func_088DA060(void *effect);
extern void func_0887BA5C(void *window);

void func_08AF5A04(MenuPage *self)
{
    for (int i = 0; i < 3; i++) {
        if (self->effects[i] != 0) {
            func_088DA060(self->effects[i]);
            self->effects[i] = 0;
        }
    }
    if (self->window != 0) {
        func_0887BA5C(self->window);
        self->window = 0;
    }
    self->field_818 = 0;
}

}
