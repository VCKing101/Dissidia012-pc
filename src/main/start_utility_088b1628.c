// Creates the "UTILITY" thread with entry func_088B1A04, stores it at obj+0x14, registers it and calls func_089C5D68.
typedef struct { int f0, f4, f8, fC, f10, f14; } Ut44;
extern int func_089C5F80(const char *, void *, int);
extern void func_089C57F8(int, int);
extern int func_08867B90(void);
extern int func_08867BB0(void);
extern void func_089C5D68(int, int);
extern void func_088B1A04(void);
extern char D_089CB768[];
void func_088B1628(Ut44 *o)
{
    int t = func_089C5F80(D_089CB768, (void *)func_088B1A04, 0);
    int a, b;
    o->f14 = t;
    func_089C57F8(0, t);
    a = func_08867B90();
    b = func_08867BB0();
    func_089C5D68(a, b);
}
