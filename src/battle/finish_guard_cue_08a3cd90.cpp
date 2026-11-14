/* Near the end of the finishing motion: cues the guard banner while the
 * fighter guards (this finisher's cue number; some only for the first side). */
class CueActorBase {
public:
    unsigned char pad_000[0x130];
    unsigned char motion[0x78];
    float time;
    unsigned char pad_1ac[0x398];

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

struct CueCtrl {
    CueActorBase *actor;
    unsigned char pad_04[4];
    int second;
};

extern void *D_08A405E0;

extern "C" void func_08A3DBD0(void *banner, int cue);
struct GuardBanner {
    static void show(int cue) { func_08A3DBD0(D_08A405E0, cue); }
};

extern "C" {

extern float func_0882FC20(void *motion, int frame);
extern void func_08A3DBD0(void *banner, int cue);

void func_08A3CD90(CueCtrl *self)
{
    float end = func_0882FC20(self->actor->motion, -1);
    CueActorBase *actor = self->actor;
    if (end - actor->time < 60.0f && actor->guard()) {
        GuardBanner::show(1);
    }
}

}
