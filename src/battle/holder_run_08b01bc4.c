typedef struct BattleHolder {
    char pad_00[0x4];
    void *battle;
} BattleHolder;

extern int func_08B01924(void *battle);

void func_08B01BC4(BattleHolder *self)
{
    if (self == 0) {
        return;
    }
    if (self->battle == 0) {
        return;
    }
    if (func_08B01924(self->battle) != 0) {
        return;
    }
}
