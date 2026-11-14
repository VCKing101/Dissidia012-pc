typedef struct MenuBattleActor {
    char pad_000[0x3B0];
    float field_3B0;
} MenuBattleActor;

typedef struct MenuBattleScene {
    char pad_000[0x3C];
    MenuBattleActor *field_3C;
} MenuBattleScene;

typedef struct MenuBattleView {
    char pad_00[0x48];
    MenuBattleScene *field_48;
} MenuBattleView;

int func_08A31E6C(MenuBattleView *self) {
    MenuBattleActor *actor = self->field_48->field_3C;
    if (actor == 0) {
        return 1;
    }
    if (actor->field_3B0 <= 0.0f) {
        return 1;
    }
    return 0;
}
