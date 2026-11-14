typedef struct MenuBattleInfo {
    char pad_00[0xC];
    unsigned short field_0C;
} MenuBattleInfo;

typedef struct MenuBattleState {
    int field_00;
    int field_04;
    char pad_008[0x8C];
    int field_94;
    char pad_098[0x550 - 0x98];
    unsigned short field_550[0xAA];
    int field_6A4;
    int field_6A8;
} MenuBattleState;

typedef struct MenuBattleOwner {
    char pad_000[0x6DC];
    MenuBattleState *field_6DC;
} MenuBattleOwner;

typedef struct MenuBattleGlobal {
    char pad_000[0x134];
    int field_134;
} MenuBattleGlobal;

extern char D_08A35578[];
extern char D_08A3566C[];
extern void *D_08A35A00;
extern MenuBattleGlobal *D_08A457EC;
extern void func_088D3B88(int a, int b, char *table, int c, int d);
extern void func_088D3CF0(int a, int b, char *table);
extern MenuBattleInfo *func_08A3C110(unsigned short id);
extern void func_08A311DC(void *object, int handle, MenuBattleInfo *info, int index);

void func_08A30A20(MenuBattleOwner *self) {
    MenuBattleInfo *info;

    self->field_6DC->field_6A4 = 0;
    func_088D3B88(self->field_6DC->field_00, self->field_6DC->field_04, D_08A35578, 0, 0x120);
    func_088D3CF0(self->field_6DC->field_00, self->field_6DC->field_04, D_08A3566C);
    info = func_08A3C110(self->field_6DC->field_550[self->field_6DC->field_94]);
    func_08A311DC(D_08A35A00, D_08A457EC->field_134, info, self->field_6DC->field_94);
    self->field_6DC->field_6A4 |= 0x10;
    self->field_6DC->field_6A8 = 9;
}
