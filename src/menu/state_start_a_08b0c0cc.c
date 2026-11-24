/* Fills the state block at 0x09B6D868 (mode 0) and calls the 0x088650A4 hook on self + 0x100. */
struct St { int a, mode, c; char pad[0x164 - 12]; int d, e, f; };
extern struct St D_09B6D868;
extern void func_088650A4(char *p);
void func_08B0C0CC(char *self, int a, int e, int f)
{
    D_09B6D868.a = a;
    D_09B6D868.mode = 0;
    D_09B6D868.d = 0;
    D_09B6D868.e = e;
    D_09B6D868.f = f;
    func_088650A4(self + 0x100);
}
