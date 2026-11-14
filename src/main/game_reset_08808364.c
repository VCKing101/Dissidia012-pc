typedef struct Agent4Pair {
    int field_00;
    int field_04;
} Agent4Pair;

extern Agent4Pair D_089D2800;
extern unsigned char D_08B63280;

void func_0880AEBC(void);
void func_08828450(void);
void func_0880766C(void);

void func_08808364(void) {
    func_0880AEBC();
    func_08828450();
    D_089D2800.field_04 = 0;
    D_089D2800.field_00 = 0;
    D_08B63280 = 1;
    func_0880766C();
}
