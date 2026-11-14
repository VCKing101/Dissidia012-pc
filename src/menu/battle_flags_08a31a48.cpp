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
} MenuBattleScene;

typedef struct MenuBattleView {
    char pad_00[0x48];
    MenuBattleScene *field_48;
} MenuBattleView;

extern "C" bool func_08A31A48(MenuBattleView *self) {
    if (self->field_48->field_3C->field_550->field_02 != 1) {
        return true;
    }
    return false;
}
