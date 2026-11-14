typedef struct BattleSlot {
    char data[0x50];
} BattleSlot;

extern "C" {

extern BattleSlot D_09B49140[2];
extern void func_08AEEDA8(BattleSlot *slot);

void func_08AEF12C(void)
{
    for (int i = 0; i < 2; i++) {
        func_08AEEDA8(&D_09B49140[i]);
    }
}

}
