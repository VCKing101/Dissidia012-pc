struct ResetScene {
    char pad_00[0xEE0];
    int field_EE0;
};

extern "C" {

extern int D_089D7280;
extern int D_089D727C;
extern ResetScene *D_089D712C;
extern int D_089D5FC8;
extern int D_089D5FCC;
void func_088729C0(void);
void func_088B16FC(int a);
int func_0887AFBC(int a, int b, int c);
void func_088B1230(int a, int b, int c, int d, int e, int f);
void func_088F4870(int id, int value);

void func_088705F0(void)
{
    func_088729C0();
    func_088B16FC(D_089D7280);
    int v = func_0887AFBC(D_089D712C->field_EE0, 0x1E, 0);
    func_088B1230(D_089D727C, v, 0, 0, 0xA0, 0);
    {
        int zero = 0;
        func_088F4870(0x14, zero);
    }
    D_089D5FC8 = 0;
    D_089D5FCC = 0;
}

}
