typedef struct MenuBoard {
    char pad_00[0x2C];
    void *effects[66];
    void *windows[10];
} MenuBoard;

extern "C" {

extern void func_088DA060(void *effect);
extern void func_0887BA5C(void *window);

void func_08AF6AE4(MenuBoard *self)
{
    for (int i = 0; i < 66; i++) {
        if (self->effects[i] != 0) {
            func_088DA060(self->effects[i]);
            self->effects[i] = 0;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (self->windows[i] != 0) {
            func_0887BA5C(self->windows[i]);
            self->windows[i] = 0;
        }
    }
}

}

