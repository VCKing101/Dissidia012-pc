// Builds a 52-byte record on the stack, hands it to one of two functions with D_089D8528 depending on the flag, then finishes the record.
extern "C" {
extern void *D_089D8528;
extern void func_0892262C(char *rec);
extern void func_089227E0(char *rec, int a);
extern void func_0891FCA0(void *o, char *rec);
extern void func_0891FBEC(void *o, char *rec);
extern void func_08922654(char *rec, int n);
}
struct H {
static void run(int unused, int a, int flag)
{
    char rec[0x34];
    func_0892262C(rec);
    func_089227E0(rec, a);
    if (flag != 0) { func_0891FCA0(D_089D8528, rec); }
    else { func_0891FBEC(D_089D8528, rec); }
    func_08922654(rec, 2);
}
};
extern "C" void func_08B39EAC(int unused, int a, unsigned char flag) { H::run(unused, a, flag); }
