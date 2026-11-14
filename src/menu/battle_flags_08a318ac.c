typedef struct MenuBattleRule {
    char pad_0[0x2];
    unsigned char field_02;
} MenuBattleRule;

typedef struct MenuBattleActor {
    char pad_000[0x550];
    MenuBattleRule *field_550;
} MenuBattleActor;

typedef struct MenuBattleScene {
    char pad_000[0x3C];
    MenuBattleActor *field_3C;
    char pad_040[0x164 - 0x40];
    int field_164;
} MenuBattleScene;

typedef struct MenuBattleView {
    char pad_00[0x48];
    MenuBattleScene *field_48;
} MenuBattleView;

int func_08A318AC(MenuBattleView *self) {
    if (self->field_48->field_164 == 3) {
        return 1;
    }
    return 0;
}
