/* Starts a battle phase: marks the side whose turn it is on the banner and
 * hands the next turn to the root. */
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

class BattlePhase : public PhaseBase {
public:
    unsigned char pad_04[4];
    int state;
    unsigned char ready;
    unsigned char shown;
    unsigned char pad_0e[2];
    float time;
    unsigned char pad_14[0x134];
    int wins;
    int losses;
    unsigned char pad_150[4];
    void *banner;
};

struct RootHolder {
    unsigned char pad_000[0x7A4];
    void *root;
};

extern "C" {

extern RootHolder *D_089D6A10;
extern void func_089F5974(void *banner, int *marks, int flags);
extern void func_089DDB50(void *root, int side, int flags);

int func_089F0104(BattlePhase *phase)
{
    if (!phase->is_active()) {
        return -1;
    }
    if (phase->state != -1) {
        return -2;
    }
    int marks[3];
    for (int i = 0; i < 3; i++) {
        marks[i] = 2;
    }
    int current = (phase->wins + phase->losses) % 2;
    int next = (current + 1) % 2;
    int active = 1;
    marks[current] = active;
    func_089F5974(phase->banner, marks, 0);
    void *root = D_089D6A10->root;
    if (root == 0) {
        return -3;
    }
    func_089DDB50(root, next, 1);
    phase->state = 0;
    phase->ready = 0;
    phase->shown = 0;
    phase->time = 0.0f;
    return 0;
}

}
