/* Per-frame update of the two pair entries: follows the battle's pair mode
 * and shows or hides the pair displays. */
struct PairEntry {
    unsigned char data[0xD8];
};

struct PairSet {
    unsigned char open;
    unsigned char pad_01[7];
    PairEntry entries[2];
    unsigned char pad_1b8;
    unsigned char hidden;
    unsigned char pad_1ba[2];
    unsigned char paired;
};

struct BattleRules {
    static bool wide(BattleRules *r) { return r->pair_mode == 4 || r->pair_mode == 5; }
    unsigned char pad_000[0x278];
    int pair_mode;
};

extern "C" {

extern BattleRules *D_08A405E0;
extern int func_089E3B0C(PairEntry *entry);
extern void func_089E3C3C(PairEntry *entry);
extern int func_089E3618(PairSet *set);
extern void func_089E37C0(PairSet *set, int layer);
extern void func_089E3768(PairSet *set, int layer);
extern int func_089E9B18(void);

int func_089E3414(PairSet *set)
{
    if (!set->open) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089E3B0C(&set->entries[i]) != 0) {
            return -2;
        }
    }
    unsigned char paired = D_08A405E0->pair_mode != 0;
    if (paired != set->paired) {
        if (paired == 1) {
            for (int i = 0; i < 2; i++) {
                func_089E3C3C(&set->entries[i]);
            }
        }
        set->paired = paired;
    }
    if (BattleRules::wide(D_08A405E0) == 1) {
        if (!set->hidden) {
            func_089E3618(set);
        }
        func_089E37C0(set, 1);
    } else {
        func_089E3768(set, 1);
    }
    if (func_089E9B18() == 1 && !paired) {
        func_089E37C0(set, 2);
    } else {
        func_089E3768(set, 2);
    }
    return 0;
}

}
