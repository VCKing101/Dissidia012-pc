/* Runs the close routines on the sub-objects of self and the other, then notifies the manager. */
extern int D_089D7F90;
extern void func_08A73770(int p);
extern void func_08A72998(int p);
extern void func_08A72760(int p);
extern void func_08A72800(int p);
extern void func_088FCA30(int v);
void func_08A786FC(int self, int other) {
    int o = other;
    int s = self;
    func_08A73770(s + 4);
    func_08A72998(s + 0x154);
    func_08A72760(o + 0x184);
    func_08A72800(o + 0x184);
    func_088FCA30(D_089D7F90);
}
