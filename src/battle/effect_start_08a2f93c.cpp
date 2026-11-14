/* Starts the hit effect: clears its parts, cues the guard banner when the
 * fighter guards, plays the start motion and loads the sprites. */
struct StartModelPart {
    unsigned char pad_00[0x28];
    void *bone;
};

struct StartModel {
    unsigned char pad_00[0x34];
    StartModelPart part;
};

class StartActorBase {
public:
    unsigned char pad_000[0x130];
    unsigned char motion[0x260];
    StartModel *model;
    unsigned char pad_394[0x134];
    unsigned char hit_point[4];
    unsigned char pad_4cc[0x78];

    virtual void slot_000();
    virtual void slot_001();
    virtual void slot_002();
    virtual void slot_003();
    virtual void slot_004();
    virtual void slot_005();
    virtual void slot_006();
    virtual void slot_007();
    virtual void slot_008();
    virtual void slot_009();
    virtual void slot_010();
    virtual void slot_011();
    virtual void slot_012();
    virtual void slot_013();
    virtual void slot_014();
    virtual void slot_015();
    virtual void slot_016();
    virtual void slot_017();
    virtual void slot_018();
    virtual void slot_019();
    virtual void slot_020();
    virtual void slot_021();
    virtual void slot_022();
    virtual void slot_023();
    virtual void slot_024();
    virtual void slot_025();
    virtual void slot_026();
    virtual void slot_027();
    virtual void slot_028();
    virtual void slot_029();
    virtual void slot_030();
    virtual void slot_031();
    virtual void slot_032();
    virtual void slot_033();
    virtual void slot_034();
    virtual void slot_035();
    virtual void slot_036();
    virtual void slot_037();
    virtual void slot_038();
    virtual int guard();
};

class StartActor : public StartActorBase {
public:
    unsigned char pad_548[0x64];
    void *guard_display;
};

struct StartParts {
    int state;
    int count;
    float time;
    float size;
    float spin;
    float fade;
    unsigned char pad_18[4];
    void *trail;
};

struct StartEffect {
    StartActor *actor;
    unsigned char pad_04[4];
    int second;
    unsigned char pad_0c[8];
    StartParts *parts;
};

extern void *D_08A405E0;

extern "C" {

extern void func_08935828(void *dest, int value, int size);
extern void func_08A3DCA8(void *banner);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void *func_088A9DE8(void *bone, int id, void *point, int a, int b);
extern void func_08A2F3F8(StartEffect *self);
extern void func_08B3AE14(float delay, void *display, int time, int a, int b);
extern void func_08A2F4E4(StartEffect *self);

void func_08A2F93C(StartEffect *self)
{
    func_08935828(self->parts, 0, 100);
    if (self->actor->guard()) {
        func_08A3DCA8(D_08A405E0);
    }
    int id = 0x1FA5;
    func_0882FA10(0.0f, 0.0f, self->actor->motion, id, 0);
    self->parts->spin = 0.0f;
    self->parts->time = 0.0f;
    self->parts->count = 0;
    self->parts->fade = 0.0f;
    self->parts->size = self->second == 0 ? 120.0f : 100.0f;
    if (self->second == 0) {
        StartActor *actor = self->actor;
        StartModelPart *part = &actor->model->part;
        self->parts->trail = func_088A9DE8(part->bone, 0x2E, actor->hit_point, 0, 0);
    }
    func_08A2F3F8(self);
    if (self->second == 0) {
        func_08B3AE14(0.0f, self->actor->guard_display, 10000, 0, 5);
    }
    func_08A2F4E4(self);
    self->parts->state = 1;
}

}
