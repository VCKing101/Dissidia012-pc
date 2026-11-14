/* Clears a sprite's three state bits and frees its data block. */
typedef struct SpriteFlags {
    unsigned int visible : 1;
    unsigned int active : 1;
    unsigned int loaded : 1;
    unsigned int rest : 29;
} SpriteFlags;

typedef struct Sprite {
    SpriteFlags flags;          /* 0x00 */
    int field_04;
    int field_08;
    void *data;                 /* 0x0C */
} Sprite;

extern void *func_08B42B40(void);
extern void func_0885C538(void *heap, void *data, int flags);

void func_089211C8(Sprite *sprite)
{
    sprite->flags.visible = 0;
    sprite->flags.active = 0;
    sprite->flags.loaded = 0;
    func_0885C538(func_08B42B40(), sprite->data, 0);
    sprite->data = 0;
}
