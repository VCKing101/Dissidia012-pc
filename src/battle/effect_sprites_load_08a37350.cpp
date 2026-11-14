/* Loads a hit effect's sprites from its sheet: the core, the flash (frame
 * 6), twelve sparks and the base (frame 1). */
struct LoadSprite {
    unsigned char pad_00[0xE];
    short frame;
};

struct LoadParts {
    unsigned char pad_00[0xAC];
    LoadSprite *base;
    LoadSprite *flash;
    void *core;
    void *sparks[12];
};

struct LoadEffect {
    unsigned char pad_00[0xC];
    void *sheet;
    unsigned char pad_10[4];
    LoadParts *parts;
};

extern "C" {

extern void *func_088DAA40(void *sheet, int frame, int flags);
extern void *func_088DAA68(void *sheet);

void func_08A37350(LoadEffect *self)
{
    int i;
    self->parts->core = func_088DAA68(self->sheet);
    self->parts->flash = (LoadSprite *)func_088DAA40(self->sheet, 5, 0);
    self->parts->flash->frame = 6;
    for (i = 0; i < 12; i++) {
        self->parts->sparks[i] = func_088DAA68(self->sheet);
    }
    self->parts->base = (LoadSprite *)func_088DAA40(self->sheet, 0, 0);
    self->parts->base->frame = 1;
}

}
