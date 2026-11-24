/* Initialises the member at +0x14 (arguments passed through) and attaches it to the fifth argument. */
extern void func_088D3278(int p, int a, int b, int c, int d, int e);
extern void func_088D3374(int p, int a, int b);
extern void func_088D3068(int owner, int p);
void func_08A76AB0(int self, int a, int b, int c, int owner) {
    int o = owner;
    int p = self + 0x14;
    func_088D3278(p, a, b, c, 0, 0);
    func_088D3374(p, 1, 4);
    func_088D3068(o, p);
}
