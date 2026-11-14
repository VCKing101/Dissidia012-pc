/* Per-frame update of the duel round's displays. */
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

struct MatchBadge {
    unsigned char data[0x34];
};

class DuelRound : public PhaseBase {
public:
    unsigned char pad_04[0x14];
    unsigned char board[0x90];
    unsigned char gauge[0x38];
    MatchBadge badges[2];
};

extern "C" {

extern void func_08A09FF8(DuelRound *phase, float x, float y);
extern void func_089F8980(void *board);
extern int func_089F83AC(void *gauge);
extern void func_08A08334(MatchBadge *badge);

int func_08A09C88(DuelRound *phase, float x, float y)
{
    if (!phase->is_active()) {
        return -1;
    }
    func_08A09FF8(phase, x, y);
    func_089F8980(phase->board);
    func_089F83AC(phase->gauge);
    for (int i = 0; i < 2; i++) {
        func_08A08334(&phase->badges[i]);
    }
    return 0;
}

}
