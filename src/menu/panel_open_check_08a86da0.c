/* Runs the base routine; when the global lookup fails and the second check passes, runs two follow-ups. */
extern int D_089D6998;
extern int D_089D712C;
extern void func_08A87040(int self);
extern void func_08A87060(int self);
extern int func_0887E718(int g);
extern int func_088AC9A8(int g);
extern void func_08865158(int p);
void func_08A86DA0(int self) {
    func_08A87040(self);
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    if (func_088AC9A8(D_089D712C) == 0) {
        return;
    }
    func_08A87060(self);
    func_08865158(self + 0x100);
}
