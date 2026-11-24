// When bit 0x10 of the owner's flags is set and the global lookup fails, runs the prepare routine and stores the next value.
extern "C" {
extern int D_089D6998;
int func_0887E718(int g);
void func_08ABF48C(int self);
int func_08ABC868(int self);
}
struct Flags {
    static bool Test(unsigned w) { return (w & 0x10) != 0; }
};
extern "C" void func_08ABD03C(int self) {
    int v;
    if (!Flags::Test(*(unsigned *)(*(int *)(self + 0x14C) + 0xA58))) {
        return;
    }
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    func_08ABF48C(self);
    v = func_08ABC868(self);
    *(int *)(*(int *)(self + 0x14C) + 0xA5C) = v;
}
