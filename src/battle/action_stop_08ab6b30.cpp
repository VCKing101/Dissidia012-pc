/* An action holder with its vptr after 0x544 bytes of data; virtual N sits at vtable offset 8 * (N + 1). */
class StopHolderBase {
public:
    char pad_000[0x394];
    void *sound;
    char pad_398[0x1AC];
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058();
    virtual void slot_060();
    virtual void slot_068();
    virtual void slot_070();
    virtual void slot_078();
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0();
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0();
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual void slot_0D8();
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
    virtual void slot_100();
    virtual void slot_108();
    virtual void slot_110();
    virtual void slot_118();
    virtual void slot_120();
    virtual void slot_128();
    virtual void slot_130();
    virtual void slot_138();
    virtual void slot_140();
    virtual void slot_148();
    virtual void slot_150();
    virtual void slot_158();
    virtual void slot_160();
    virtual void stopMotion();
};

class StopHolder : public StopHolderBase {
public:
    char pad_548[0x8];
    unsigned char active;
    char pad_551[0x3];
    int state;
    int target;
    char pad_55C[0x4];
    int owner;
    char pad_564[0x8];
    float speed;
    float time;
};

extern "C" {

extern void func_08AB7520(int owner, int target);
extern void func_08AB7004(StopHolder *self, int on);
extern void func_08834DF4(StopHolder *self, float scale);
extern void func_0886B230(void *sound);
extern void func_08AB6DF0(StopHolder *self);

/* Stops the action: releases the target, halts the motion and sound and moves to state 2; -1 inactive. */
int func_08AB6B30(StopHolder *self)
{
    if (self->active == 0) {
        return -1;
    }
    func_08AB7520(self->owner, self->target);
    func_08AB7004(self, 1);
    self->stopMotion();
    func_08834DF4(self, 1.0f);
    if (self->sound != 0) {
        func_0886B230(self->sound);
    }
    self->state = 2;
    self->speed = 0.0f;
    self->time = 0.0f;
    func_08AB6DF0(self);
    return 0;
}

}
