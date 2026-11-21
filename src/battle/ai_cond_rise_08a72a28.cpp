/* AI recovery cue condition (timer 14): arms when the fighter starts to
 * recover, and while armed cues the quick rise (0xD), the roll (0x10) or
 * the jump-out (0x11) as soon as each becomes possible, disarming (-1 bad
 * arguments, -2 no fighter). */
class RiseFighter {
public:
    unsigned char pad_000[0x544];
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
    virtual void slot_039();
    virtual void slot_040();
    virtual void slot_041();
    virtual int can_jump_out();
};

struct RiseInfo {
    unsigned char pad_00[4];
    RiseFighter *fighter;
};

struct RiseAi {
    unsigned char pad_000[0x14];
    unsigned char timers[0x50];
    void *owner;
    void *cues;
    unsigned char pad_06c[0x171];
    unsigned char armed;
};

extern "C" {

extern RiseInfo *func_08AB3264(void *owner);
extern int func_08A7E904(void *timers, int id);
extern void func_08A7E858(void *timers, int id);
extern int func_08838074(RiseFighter *fighter);
extern int func_08B2F95C(RiseFighter *fighter);
extern int func_08B3A5BC(RiseFighter *fighter, int mode);
extern void func_08A97060(void *cues, int cue);

int func_08A72A28(RiseAi *self, void *cond, void *key, void *flag)
{
    if (cond == 0 || key == 0 || flag == 0) {
        return -1;
    }
    RiseFighter *fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -2;
    }
    {
        int id = 0xE;
        void *timers = self->timers;
        int one;
        int running = func_08A7E904(timers, id);
        one = 1;
        if (running == 0) {
            self->armed = 0;
        }
        func_08A7E858(timers, id);
        if (func_08838074(fighter) == one) {
            self->armed = 1;
        }
        if (self->armed == one) {
            if (func_08B2F95C(fighter) == one) {
                func_08A97060(self->cues, 0xD);
                self->armed = 0;
            }
            if (func_08B3A5BC(fighter, 0) == one) {
                func_08A97060(self->cues, 0x10);
                self->armed = 0;
            }
            if (fighter->can_jump_out() == one) {
                func_08A97060(self->cues, 0x11);
                self->armed = 0;
            }
        }
    }
    return 0;
}

}
