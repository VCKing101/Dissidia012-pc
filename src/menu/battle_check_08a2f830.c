typedef struct MenuBattleInfo {
    char pad_00[0xC];
    unsigned short field_0C;
} MenuBattleInfo;

typedef struct MenuBattleState {
    char pad_000[0x94];
    int field_94;
    char pad_098[0x550 - 0x98];
    unsigned short field_550[1];
} MenuBattleState;

typedef struct MenuBattleOwner {
    char pad_000[0x6DC];
    MenuBattleState *field_6DC;
} MenuBattleOwner;

extern MenuBattleInfo *func_08A3C110(unsigned short id);

int func_08A2F830(MenuBattleOwner *self, int *busy) {
    if (*busy == 0) {
        return func_08A3C110(self->field_6DC->field_550[self->field_6DC->field_94])->field_0C != 0xFFFF;
    }
    return 0;
}
