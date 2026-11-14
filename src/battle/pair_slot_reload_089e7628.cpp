/* Reloads a pair slot's two models for the battle modes that show them and
 * marks the second model as an overlay in the modes that need it. */
struct SlotModel {
    unsigned char pad_00[0x20];
    unsigned short flags;
};

struct BattleSession {
    unsigned char pad_000[0x48];
    int mode;

    static bool is(BattleSession *session, int mode) { return session->mode == mode; }
};

struct StageInfo {
    unsigned char pad_00[0x8];
    int stage;
};

struct PairSlot {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    SlotModel *first;
    SlotModel *second;
    int side;
    void *source;
    int pair;
};

extern "C" {

extern BattleSession *D_089D6A10;
extern StageInfo *D_08B0CF20;
extern void *func_089E95B0(int side);
extern void *func_08889E44(void);
extern int func_089E36E8(int pair);
extern void func_088DA060(void *model);
extern SlotModel *func_088DAA40(void *source, int name, int flags);

int func_089E7628(PairSlot *slot)
{
    void *fighter = func_089E95B0(slot->side);
    void *player = func_08889E44();
    if (func_089E36E8(slot->pair) == 1) {
        return 0;
    }
    int stage = D_08B0CF20->stage;
    if (!(BattleSession::is(D_089D6A10, 3) == 1 ||
          (BattleSession::is(D_089D6A10, 4) == 1 && (stage == 9 || stage == 10) && fighter == player))) {
        return 0;
    }
    if (slot->second != 0) {
        func_088DA060(slot->second);
        slot->second = 0;
    }
    if (slot->first != 0) {
        func_088DA060(slot->first);
        slot->first = 0;
    }
    slot->first = func_088DAA40(slot->source, 0, 0);
    slot->second = func_088DAA40(slot->source, 1, 0);
    if (BattleSession::is(D_089D6A10, 4) == 1 || BattleSession::is(D_089D6A10, 7) == 1 ||
        BattleSession::is(D_089D6A10, 8) == 1) {
        if (slot->second != 0) {
            slot->second->flags |= 0x2000;
        }
    }
    slot->state = 2;
    return 0;
}

}
