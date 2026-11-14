struct EffectPos {
    char pad_00[0x4];
    float x;
    float y;
};

struct SoundBank34F168 {
    char pad_00[0x28];
    void *bank;

    static void *bankOf(const SoundBank34F168 &sub) { return sub.bank; }
};

struct EffectSceneF168 {
    char pad_000[0x34];
    SoundBank34F168 field_34;
};

struct EffectRootF168 {
    char pad_000[0x390];
    EffectSceneF168 *scene;
};

struct EffectBinder {
    EffectRootF168 *root;
    char pad_04[0x10];
    EffectPos *pos;
};

extern "C" {

extern char *D_08B3D6B8;
extern void func_08935828(void *dst, int value, int size);
extern void func_088AA114(void *bank, int id);
extern float func_0887357C(void *target);
extern float func_08873584(void *target);

void func_08A4F168(EffectBinder *self, EffectPos *pos)
{
    void *bank;
    char *target;
    self->pos = pos;
    func_08935828(pos, 0, 0xC);
    bank = SoundBank34F168::bankOf(self->root->scene->field_34);
    if (bank != 0) {
        func_088AA114(bank, 0x22B8);
    }
    target = D_08B3D6B8 + 0xC0;
    self->pos->x = func_0887357C(target);
    self->pos->y = func_08873584(target);
}

}
