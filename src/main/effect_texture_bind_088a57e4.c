/* Uploads an effect's texture on first use and keeps its handle. */
typedef struct DrawCtx {
    char pad_00[8];
    int textures;
} DrawCtx;

typedef struct EffectTex {
    char pad_0000[0x1248];
    int bank;
    int texture;
} EffectTex;

extern DrawCtx *D_089D6D80;

extern void *func_088A5208(int bank);
extern int func_089411D8(int textures, void *data);

void func_088A57E4(EffectTex *self)
{
    if (self->texture == -1) {
        EffectTex *keep[1];
        void *data;
        keep[0] = self;
        data = func_088A5208(keep[0]->bank);
        keep[0]->texture = func_089411D8(D_089D6D80->textures, data);
    }
}
