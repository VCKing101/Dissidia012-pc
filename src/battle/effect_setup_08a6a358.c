typedef struct EffectOwner {
    unsigned char active;
    char pad_01[0x6F];
    int effect;
} EffectOwner;

extern void func_08A65A84(int effect);
extern void func_08A65AAC(int effect);
extern void func_08A65B50(int effect, int frames);
extern void func_08A65C08(int effect, int value);
extern void func_08A65AFC(int effect);

int func_08A6A358(EffectOwner *self, unsigned int kind, unsigned char variant)
{
    if (self->active == 0) {
        return -1;
    }
    if (kind >= 3) {
        return -2;
    }
    func_08A65A84(self->effect);
    func_08A65AAC(self->effect);
    func_08A65B50(self->effect, 0x3C);
    func_08A65C08(self->effect, kind);
    func_08A65C08(self->effect, variant);
    func_08A65AFC(self->effect);
    return 0;
}
