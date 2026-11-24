/* Unless the owner's field at +0x5E4 is 10000, runs both reset routines over their slot ids. */
extern void func_08A7F59C(int self, int slot, int v);
extern void func_08A7F398(int self, int slot, int v);
void func_08A7FA7C(int self) {
    if (*(int *)(*(int *)(self + 0x28) + 0x5E4) == 10000) {
        return;
    }
    func_08A7F59C(self, 3, -1);
    func_08A7F59C(self, 2, -1);
    func_08A7F398(self, 6, -1);
    func_08A7F398(self, 4, -1);
    func_08A7F398(self, 5, -1);
    func_08A7F398(self, 2, -1);
    func_08A7F398(self, 3, -1);
    func_08A7F398(self, 7, -1);
    func_08A7F59C(self, 0, -1);
    func_08A7F59C(self, 1, -1);
}
