typedef struct MenuBattleGlobal {
    char pad_000[0x134];
    int field_134;
} MenuBattleGlobal;

extern MenuBattleGlobal *D_08A457EC;
extern void func_0885C6CC(int handle, void *self, int a, int b, int c, int d);

void func_08A408E8(void *self) {
    int none = 0;
    func_0885C6CC(D_08A457EC->field_134, self, 0, 0, none, 0);
}
