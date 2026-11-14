typedef struct BattleHolder {
    char pad_00[0x4];
    void *battle;
} BattleHolder;

extern int func_08B0165C(void *battle);

void func_08B01B74(BattleHolder *self)
{
    if (self == 0) {
        return;
    }
    if (self->battle == 0) {
        return;
    }
    if (func_08B0165C(self->battle) != 0) {
        return;
    }
}
