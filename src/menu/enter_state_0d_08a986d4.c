/* Sets the owner's state to 0xD, runs the setup steps, handles the two flag bits of the owner's word at +0x81B4 and runs the final step. */
extern char D_09B6EFB0[];
extern void func_08AA0FBC(int self);
extern void func_08A9CF8C(int self);
extern void func_08A9C55C(int self, int a);
extern void func_08A9E5E0(int self);
extern void func_08AA56C8(int p);
extern void func_08A9BEA0(int self);
void func_08A986D4(int self) {
    int o;
    *(int *)(*(int *)(self + 0x154) + 0x910) = 0xD;
    func_08AA0FBC(self);
    func_08A9CF8C(self);
    func_08A9C55C(self, 4);
    *(int *)(*(int *)(self + 0x154) + 0x81C4) = 4;
    func_08A9E5E0(self);
    o = *(int *)(self + 0x154);
    if ((*(unsigned *)(o + 0x81B4) & 0x10000) != 0) {
        func_08AA56C8(*(int *)(o + 0x978));
        *(unsigned *)(*(int *)(self + 0x154) + 0x81B4) = *(unsigned *)(*(int *)(self + 0x154) + 0x81B4) & 0xFFFEFFFF;
        o = *(int *)(self + 0x154);
    }
    if ((*(unsigned *)(o + 0x81B4) & 0x20) != 0) {
        unsigned *g = (unsigned *)(D_09B6EFB0 + 0x304);
        g[0x4B] = g[0x4B] | 1;
    }
    func_08A9BEA0(self);
}
