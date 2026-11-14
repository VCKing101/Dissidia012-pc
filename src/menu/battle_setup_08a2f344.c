typedef struct MenuBattleEntry {
    char data[0x28];
} MenuBattleEntry;

typedef struct MenuBattleState {
    char pad_000[0xC];
    char field_0C[0x58];
    char field_64[0x2C];
    char field_90[0x620];
    int field_6B0;
} MenuBattleState;

typedef struct MenuBattleOwner {
    char pad_000[0x6DC];
    MenuBattleState *field_6DC;
} MenuBattleOwner;

extern MenuBattleEntry D_08A35878[];
extern void func_088D4C64(char *a, char *b, char *c, int d, int e, int f);
extern void func_08A3248C(MenuBattleOwner *self, MenuBattleEntry *entry, int mode);
extern void func_08A2F134(MenuBattleOwner *self);

void func_08A2F344(MenuBattleOwner *self) {
    MenuBattleState *state = self->field_6DC;
    func_088D4C64(state->field_90, state->field_0C, state->field_64, 0, 1, 2);
    func_08A3248C(self, &D_08A35878[self->field_6DC->field_6B0], 10);
    func_08A2F134(self);
}
