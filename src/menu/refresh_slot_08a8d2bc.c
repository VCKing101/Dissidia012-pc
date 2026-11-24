/* Picks the id for the current mode (the argument, or -1 -> the table short selected by the owner's entries), runs the two refresh steps with it. */
extern short D_08A93A32[];
extern short D_08A93A7A[];
extern void func_08A8DF30(int self, int a);
extern void func_08A8DB4C(int self, int a, int b, int c);
void func_08A8D2BC(int self, int v) {
    int keep[2];
    int owner;
    int mode;
    int e;
    short s;
    owner = *(int *)(self + 8);
    mode = *(int *)(owner + *(int *)(owner + 0x2B4) * 4 + 0x308);
    if (v != -1) {
        keep[0] = v;
    } else {
        e = *(int *)(owner + mode * 0x2CC + *(int *)(owner + mode * 4 + 0x2BC) * 4 + 0x628);
        if (mode == 0) {
            s = *(short *)(e * 8 + (int)D_08A93A32);
        } else {
            s = *(short *)(e * 8 + (int)D_08A93A7A);
        }
        keep[0] = s;
    }
    func_08A8DF30(self, 10);
    func_08A8DB4C(self, 10, keep[0], -1);
}
