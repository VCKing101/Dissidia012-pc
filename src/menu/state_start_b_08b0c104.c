/* Fills the state block at 0x09B6D868 (mode 1) and calls the 0x088650A4 hook on self + 0x100. */
struct St { int a, mode, c; char pad[0x164 - 12]; int d, e, f; };
extern struct St D_09B6D868;
extern void func_088650A4(char *p);
void func_08B0C104(char *self, int c, int a)
{
    D_09B6D868.a = a;
    D_09B6D868.mode = 1;
    D_09B6D868.c = c;
    D_09B6D868.d = 0;
    func_088650A4(self + 0x100);
}
