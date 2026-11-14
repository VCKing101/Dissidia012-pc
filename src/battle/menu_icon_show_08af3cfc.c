typedef struct MenuSprite {
    char pad_00[0xE];
    short shown;
} MenuSprite;

typedef struct MenuIcon {
    int pad_00;
    MenuSprite *sprite;
} MenuIcon;

extern void func_088D9F5C(MenuSprite *sprite, int id, unsigned char flag);

void func_08AF3CFC(MenuIcon *self)
{
    if (self->sprite != 0) {
        func_088D9F5C(self->sprite, 0, 1);
        self->sprite->shown = 1;
    }
}
