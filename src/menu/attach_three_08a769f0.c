/* Initialises the member at +8, derives two more from it, sets each up with (1, 4) and attaches them to the owners. */
extern void func_088D3278(int p, int a, int b, int c, int d, int e);
extern void func_088D32F8(int p, int src);
extern void func_088D3374(int p, int a, int b);
extern void func_088D3068(int owner, int p);
void func_08A769F0(int self, int a, int b, int c) {
    int p1 = self + 8;
    int p2;
    int p3;
    func_088D3278(p1, a, b, c, 0, 0);
    p2 = self + 0xC;
    func_088D32F8(p2, p1);
    p3 = self + 0x10;
    func_088D32F8(p3, p1);
    func_088D3374(p1, 1, 4);
    func_088D3374(p2, 1, 4);
    func_088D3374(p3, 1, 4);
    func_088D3068(self, p1);
    func_088D3068(self, p2);
    func_088D3068(self + 4, p3);
}
