typedef struct MenuSprite {
    char pad_00[0x20];
    unsigned short flags;

    static bool hidden(MenuSprite *sprite) { return (sprite->flags & 0x20) != 0; }
} MenuSprite;

typedef struct MenuIcon {
    int pad_00;
    MenuSprite *sprite;
} MenuIcon;

extern "C" {

unsigned char func_08AF3DD4(MenuIcon *self)
{
    int result = 0;
    if (self->sprite != 0 && MenuSprite::hidden(self->sprite) == 1) {
        result = 1;
    }
    return result;
}

}
