
/* Burst effect finish: frees the trail, spawns the finishing burst on the
 * fighter's hit point (aimed at the target, or the scaled burst towards the
 * target's voice bone), stops the charge effect when the burst was cut,
 * restores the camera's range and the screen shake. */
struct BurstVoicePart {
    unsigned char pad_00[0x18];
    int bone;
};

struct BurstVoice {
    unsigned char pad_00[0xA4];
    BurstVoicePart part;

    static int bone_of(BurstVoice *voice)
    {
        BurstVoicePart *part = &voice->part;
        return part->bone;
    }
};

struct BurstModelPart {
    unsigned char pad_00[0x28];
    void *bone;
};

struct BurstModel {
    unsigned char pad_00[0x34];
    BurstModelPart part;
};

struct BurstActor {
    unsigned char pad_000[0x390];
    BurstModel *model;
    unsigned char pad_394[0x134];
    unsigned char hit_point[4];
    unsigned char pad_4cc[0x898];
    unsigned char effects[4];
};

struct BurstBoneEffect {
    unsigned char pad_000[0xE4];
    float scale;
};

struct BurstTarget {
    unsigned char pad_000[0x394];
    BurstVoice *voice;
    unsigned char pad_398[0x130];
    unsigned char hit_point[4];
};

struct BurstFinishParts {
    unsigned char pad_00[9];
    unsigned char kind;
    unsigned char pad_0a[2];
    float near_range;
    float far_range;
    unsigned char pad_14[4];
    void *trail;
};

struct BurstFinishEffect {
    BurstActor *actor;
    BurstTarget *target;
    unsigned char pad_08[0xC];
    BurstFinishParts *parts;
};

struct BurstFinishCamera {
    unsigned char pad_000[0xC0];
    unsigned char range[4];
};

struct BurstFinishScreen {
    unsigned char pad_000[0x190];
    unsigned char shake[4];
};

extern "C" BurstBoneEffect *func_088A9DE8(void *bone, int id, void *point, int a, int b);
extern "C" void func_088A6A38(float time, BurstBoneEffect *effect, int mode, void *point, int bone, int flags);

struct BurstSpawn {
    static BurstBoneEffect *at_hit(BurstActor *actor, void *bone, int id) { return func_088A9DE8(bone, id, actor->hit_point, 0, 0); }
    static void aim(BurstBoneEffect *effect, int mode, BurstTarget *target, int bone) { func_088A6A38(-1.0f, effect, mode, target->hit_point, bone, -1); }
};

extern float D_08A4F068;
extern BurstFinishCamera *D_08B3D6B8;
extern BurstFinishScreen *D_089D4DC8;

extern "C" {

extern void func_088A6C70(void *trail);
extern BurstBoneEffect *func_088A9DE8(void *bone, int id, void *point, int a, int b);
extern void func_088A6A38(float time, BurstBoneEffect *effect, int mode, void *point, int bone, int flags);
extern void *func_0884F75C(void *effects, int id);
extern void func_0884EEF4(void *effect, int arg);
extern void func_08873594(void *range, float near_range, float far_range);
extern void func_08858AD8(void *shake, float scale);

void func_08A4DD5C(BurstFinishEffect *self)
{
    float scale = D_08A4F068;
    void *trail = self->parts->trail;
    if (trail != 0) {
        func_088A6C70(trail);
        self->parts->trail = 0;
    }
    BurstModelPart *part = &self->actor->model->part;
    void *bone = part->bone;
    if (bone != 0) {
        if (self->parts->kind == 1) {
            BurstBoneEffect *effect = BurstSpawn::at_hit(self->actor, bone, 0x67);
            if (effect != 0) {
                BurstSpawn::aim(effect, 2, self->target, -1);
            }
        } else {
            BurstBoneEffect *effect = BurstSpawn::at_hit(self->actor, bone, 0x62);
            BurstVoice *voice = self->target->voice;
            if (effect != 0 && voice != 0) {
                effect->scale = scale;
                BurstSpawn::aim(effect, 0, self->target, BurstVoice::bone_of(voice));
            }
        }
    }
    if (self->parts->kind == 0) {
        void *charge = func_0884F75C(self->actor->effects, 0x3D);
        if (charge != 0) {
            func_0884EEF4(charge, 0);
        }
    }
    BurstFinishCamera *camera = D_08B3D6B8;
    func_08873594(camera->range, self->parts->near_range, self->parts->far_range);
    func_08858AD8(D_089D4DC8->shake, 1.0f);
}

}
