typedef struct BattleSlot {
    char data[0x50];
} BattleSlot;

extern BattleSlot D_09B49140[2];
extern void func_08AEEE0C(BattleSlot *slot);

void func_08AEF19C(void)
{
    func_08AEEE0C(&D_09B49140[1]);
}
