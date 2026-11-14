extern void func_08912498(void *self);

typedef struct SlotBank {
    char pad_000[0x110];
    int entries[8];             /* 0x110 */
    char pad_130[0x270 - 0x130];
    int groups[4];              /* 0x270 */
} SlotBank;

void func_08912444(SlotBank *bank)
{
    int i;
    for (i = 0; i < 8; i++) {
        bank->entries[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        bank->groups[i] = 0;
    }
    func_08912498(bank);
}
