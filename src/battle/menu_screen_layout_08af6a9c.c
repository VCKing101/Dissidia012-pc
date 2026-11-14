typedef struct MenuLayout {
    int words[57];
} MenuLayout;

typedef struct MenuScreen {
    char pad_000[0x15C];
    int mode;
    char pad_160[0x14];
    MenuLayout layout;
} MenuScreen;

void func_08AF6A9C(MenuScreen *self, int mode, MenuLayout *layout)
{
    self->mode = mode;
    self->layout = *layout;
}
