typedef struct BattleHolder {
    char pad_00[0x4];
    void *battle;
} BattleHolder;

extern int func_08B04F20(void *battle);

void func_08B04ED0(BattleHolder *self)
{
    if (self == 0) {
        return;
    }
    if (self->battle == 0) {
        return;
    }
    if (func_08B04F20(self->battle) != 0) {
        return;
    }
}
