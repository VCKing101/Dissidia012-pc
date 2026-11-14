typedef struct Agent4Pair {
    int field_00;
    int field_04;
} Agent4Pair;

extern int D_089D4750[8];
extern int D_08B63284;
extern Agent4Pair D_089D4770;

void func_0880AEBC(void) {
    int i;
    for (i = 0; i < 8; i++) {
        D_089D4750[i] = 0;
    }
    D_08B63284 = 0;
    D_089D4770.field_04 = 0;
    D_089D4770.field_00 = 0;
}
