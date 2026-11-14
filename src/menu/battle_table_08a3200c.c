typedef struct MenuBattleView {
    char pad_00[0x44];
    int field_44;
} MenuBattleView;

extern short D_08A358FC[];

int func_08A3200C(MenuBattleView *self) {
    return D_08A358FC[self->field_44];
}
