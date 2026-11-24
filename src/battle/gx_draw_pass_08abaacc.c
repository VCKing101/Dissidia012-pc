// Draws the active item with the fixed render state sequence and restores the saved state; -1 when inactive.
struct Item { unsigned char active; char d1[7]; int f8; int cur; int a[2]; int b[2]; };
extern int func_089A04E8(void);
extern void func_089A296C(int v);
extern void func_089A250C(void);
extern void func_089A2568(void);
extern void func_089A25C0(void);
extern void func_0899D2F8(int v);
extern void func_089A12A4(int a, int b, int c, int d, unsigned int e);
extern void func_089A0384(int v);
extern void func_089A03DC(int v);
extern void func_089A119C(int v);
extern void func_089A0C8C(unsigned int v);
extern void func_089A0520(int a, int b, int c, int d, int e);
extern void func_0899FDC4(int v);

int func_08ABAACC(struct Item *it)
{
    int saved;
    if (it->active == 0) {
        return -1;
    } else {
    }
    saved = func_089A04E8();
    func_089A296C(2);
    func_089A250C();
    func_089A2568();
    func_089A25C0();
    func_0899D2F8(it->f8);
    func_089A12A4(0, 2, 10, 0, -1);
    func_089A0384(4);
    func_089A0384(9);
    func_089A03DC(5);
    func_089A03DC(10);
    func_089A03DC(0);
    func_089A03DC(0x11);
    func_089A03DC(3);
    func_089A03DC(7);
    func_089A119C(1);
    func_089A0C8C(-1);
    func_089A0520(6, 0x183, it->a[it->cur] * 2, 0, it->b[it->cur]);
    func_089A119C(0);
    func_0899FDC4(saved);
    return 0;
}
