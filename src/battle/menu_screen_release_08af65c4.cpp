typedef struct MenuScreen {
    char pad_00[0x10];
    void *effects[13];
    void *windows[3];
} MenuScreen;

extern "C" {

extern void func_088DA060(void *effect);
extern void func_0887BA5C(void *window);

void func_08AF65C4(MenuScreen *self)
{
    for (int i = 0; i < 13; i++) {
        if (self->effects[i] != 0) {
            func_088DA060(self->effects[i]);
            self->effects[i] = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (self->windows[i] != 0) {
            func_0887BA5C(self->windows[i]);
            self->windows[i] = 0;
        }
    }
}

}
