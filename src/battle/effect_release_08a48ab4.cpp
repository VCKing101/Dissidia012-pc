struct EffectSlots {
    char pad_00[0x24];
    float x;
    float y;
    void *field_2C[11];
    void *field_58[6];
};

struct SoundBank34 {
    char pad_00[0x28];
    void *bank;

    static void *bankOf(const SoundBank34 &sub) { return sub.bank; }
};

struct EffectScene {
    char pad_000[0x34];
    SoundBank34 field_34;
};

struct EffectRoot {
    char pad_000[0x390];
    EffectScene *scene;
};

struct EffectReleaser {
    EffectRoot *root;
    char pad_04[0x8];
    int field_0C;
    char pad_10[0x4];
    EffectSlots *slots;
};

extern "C" {

extern char *D_08B3D6B8;
extern void func_088A6C70(void *effect);
extern void func_088DA060(void *handle);
extern void func_088AA114(void *bank, int id);
extern void func_08873594(void *target, float x, float y);

void func_08A48AB4(EffectReleaser *self)
{
    void *bank;
    for (int i = 0; i < 11; i++) {
        if (self->slots->field_2C[i] != 0) {
            func_088A6C70(self->slots->field_2C[i]);
            self->slots->field_2C[i] = 0;
        }
    }
    if (self->field_0C != 0) {
        for (int i = 0; i < 6; i++) {
            if (self->slots->field_58[i] != 0) {
                func_088DA060(self->slots->field_58[i]);
                self->slots->field_58[i] = 0;
            }
        }
    }
    bank = SoundBank34::bankOf(self->root->scene->field_34);
    if (bank != 0) {
        func_088AA114(bank, 0x6C);
    }
    func_08873594(D_08B3D6B8 + 0xC0, self->slots->x, self->slots->y);
}

}
