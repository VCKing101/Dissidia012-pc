/* Resets the layout controls to their defaults. */
extern void func_08A8D480(int self, int a, int b);
extern void func_08A8DC60(int self, int a, int b);
void func_08A8BFD4(int self) {
    int v;
    int u;
    func_08A8D480(self, 0xB, -1);
    func_08A8D480(self, 4, -1);
    func_08A8D480(self, 2, -1);
    func_08A8D480(self, 3, -1);
    func_08A8D480(self, 10, -1);
    func_08A8D480(self, 9, -1);
    func_08A8D480(self, 5, -1);
    func_08A8D480(self, 6, -1);
    func_08A8D480(self, 7, -1);
    func_08A8D480(self, 8, -1);
    func_08A8DC60(self, 0, -1);
    v = 0x6E;
    u = 0x6D;
    func_08A8D480(self, 0x86, u);
    func_08A8D480(self, 0x87, v);
    func_08A8D480(self, 0x1E, -1);
    func_08A8DC60(self, 0xB, -1);
}
