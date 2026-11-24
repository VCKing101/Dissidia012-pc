/* Registers callbacks 08B31CA4/08B31CC4 on self+0x100, stores a at 0x09B6DD10 (flag byte at +4 cleared) and calls the 0x088650A4 hook. */
extern void func_0886503C(char *h, void *a, void *b, int z);
extern void func_0886504C(char *h, void *self, int size);
extern void func_088650A4(char *h);
extern void func_08B31CA4(void);
extern void func_08B31CC4(void);
struct Rec { int a; unsigned char flag; };
extern struct Rec D_09B6DD10;
void func_08B31C28(char *self, int a)
{
    char *h = self + 0x100;
    func_0886503C(h, func_08B31CA4, func_08B31CC4, 0);
    func_0886504C(h, self, 0x100);
    D_09B6DD10.a = a;
    D_09B6DD10.flag = 0;
    func_088650A4(h);
}
