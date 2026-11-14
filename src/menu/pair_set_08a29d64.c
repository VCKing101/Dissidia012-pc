struct Pair29D64 { int a; int b; };
extern struct Pair29D64 D_09B6DA10;
extern void func_088650A4(int p);

void func_08A29D64(int param_1, int param_2, int param_3)
{
    D_09B6DA10.b = param_2;
    D_09B6DA10.a = param_3;
    func_088650A4(param_1 + 0x100);
}
