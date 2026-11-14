extern int D_08AB6738;
extern char D_08AB66F8[];
extern void func_08AB54C0(int id);
extern void func_08AB5534(void);
extern void func_088823C4(int a, int b, int c, void (*callback)(void), void *data);

void func_08AB55E8(int a, int b, int c)
{
    func_08AB54C0(D_08AB6738);
    func_088823C4(a, b, c, func_08AB5534, D_08AB66F8);
}
