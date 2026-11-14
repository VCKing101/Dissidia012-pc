typedef struct MenuBattleEntry {
    char pad_00[0xC];
    int field_0C;
    char pad_10[0x18];
} MenuBattleEntry;

typedef struct MenuBattleTable {
    char pad_000[0xE70];
    int field_E70[1];
} MenuBattleTable;

typedef struct MenuBattleGlobal {
    char pad_000[0x14C];
    MenuBattleTable *field_14C;
} MenuBattleGlobal;

typedef struct MenuBattleState {
    char pad_000[0x94];
    int field_94;
    char pad_098[0x6B0 - 0x98];
    int field_6B0;
} MenuBattleState;

typedef struct MenuBattleOwner {
    char pad_000[0x6DC];
    MenuBattleState *field_6DC;
} MenuBattleOwner;

extern MenuBattleEntry D_08A35878[];
extern MenuBattleGlobal *D_08A457EC;

void func_08A2EACC(MenuBattleOwner *self) {
    MenuBattleState *state = self->field_6DC;
    int slot = D_08A35878[state->field_6B0].field_0C;
    D_08A457EC->field_14C->field_E70[slot] = state->field_94;
}
