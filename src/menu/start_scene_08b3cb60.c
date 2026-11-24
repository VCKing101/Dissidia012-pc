/* Sets two flag bytes in D_09B6DAC0 and runs the scene start-up calls on the object at D_08B41CC0. */
extern char D_09B6DAC0[];
extern void *D_08B41CC0;
extern void func_08B3E004(void *obj, int a);
extern void func_08B361D8(void *obj);
extern void func_08B363A0(void *obj, int a);
extern void func_08B363E8(void *obj, int a);
extern void func_08B3C900(int a, int b, int c, int d);
void func_08B3CB60(void)
{
    void *obj;
    D_09B6DAC0[0x21F] = 1;
    obj = D_08B41CC0;
    D_09B6DAC0[0x21E] = 1;
    func_08B3E004(obj, 1);
    func_08B361D8(obj);
    func_08B363A0(obj, 1);
    func_08B363E8(obj, 0);
    func_08B3C900(4, -1, -1, -1);
}
