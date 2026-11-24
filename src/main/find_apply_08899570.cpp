// When the flag is zero, look up the entry for the pair in the table block and register it.
struct Slot { char pad[0x108]; };
extern "C" Slot *func_088992B0(void);
extern "C" int func_08897518(Slot *tbl, int a, int b);
extern "C" void func_0889A2F0(Slot *tbl, int e);
extern "C" void func_08899570(int a, int b, unsigned flag)
{
    if (!(flag < 1)) {
        return;
    }
    Slot *s = func_088992B0() + flag;
    int e = func_08897518(s + 1, a, b);
    if (e == 0) {
        return;
    }
    func_0889A2F0(s + 1, e);
}
