typedef struct BattleRules {
    char pad_000[0x278];
    int locked;
} BattleRules;

extern "C" {
extern BattleRules *D_08A405E0;
extern void func_08AE1270(void *self, int arg);
}

struct RuleCheck {
    static bool locked() { return D_08A405E0->locked != 0; }
};

extern "C" void func_08AE1238(void *self)
{
    if (!RuleCheck::locked()) {
        func_08AE1270(self, 1);
    }
}
