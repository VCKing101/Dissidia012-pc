/* Starts the pick phase: shows each banner slot's look, marks both sides as
 * waiting and keeps the cue's current pick. */
class PhaseBase {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030();
    virtual void slot_038();
    virtual int is_active();
};

struct CueList {
    unsigned char data[0x10];
    int pick;
    unsigned char pad_14[4];
    int count;

    CueList() : count(0) {}
};

class PickPhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char pad_0d[3];
    float time;
    int look;
    int pick;
    unsigned char pad_1c[0x2C];
    unsigned char frame[0x84];
    void *cue;
    void *banner;
};

extern "C" {

extern int func_089E978C(int slot);
extern int func_08891D84(void *cue, unsigned char *shown, CueList *list, int a, int b, int c);
extern void func_089F84E0(void *frame);
extern void func_089F85B0(void *frame, int look, int flags);
extern void func_089F5974(void *banner, int *marks, int *looks);

int func_08A2A8DC(PickPhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    int looks[3];
    for (int i = 0; i < 3; i++) {
        looks[i] = func_089E978C(i);
    }
    unsigned char shown = 0;
    CueList list;
    if (func_08891D84(phase->cue, &shown, &list, 0, 0, 0) != 0) {
        return -3;
    }
    int marks[3];
    marks[2] = 0;
    for (int i = 0; i < 2; i++) {
        marks[i] = 2;
    }
    func_089F84E0(phase->frame);
    func_089F85B0(phase->frame, looks[2], 0);
    func_089F5974(phase->banner, marks, looks);
    phase->state = 0;
    phase->ready = 0;
    phase->time = 0.0f;
    phase->look = looks[2];
    phase->pick = list.pick;
    return 0;
}

}
