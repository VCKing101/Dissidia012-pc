extern "C" {
extern void func_08912444(void *bank);

typedef struct Bank {
    char data[0x280];
} Bank;

void func_08912B5C(Bank *banks)
{
    for (int i = 0; i < 5; i++) {
        func_08912444(&banks[i]);
    }
}
}
