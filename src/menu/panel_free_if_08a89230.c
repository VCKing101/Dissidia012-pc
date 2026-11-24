/* When the mode check passes and the global lookup fails, runs the release routine on self + 0x100. */
extern int D_089D6998;
extern int func_08A89F7C(void);
extern int func_0887E718(int v);
extern void func_08865158(int p);
void func_08A89230(int self) {
    if (func_08A89F7C() == 0) {
        return;
    }
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    func_08865158(self + 0x100);
}
