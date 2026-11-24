/* Writes five (x, y, w, h) short records into the owner's table at +0x9C0 from an offset and the owner's column count, then runs the setup. */
extern void func_08AB8768(int self, int p);
extern void func_088D428C(int a, int b, int c, int d);
void func_08AB8780(int self, int off) {
    int base;
    int o;
    base = off * *(int *)(*(int *)(self + 0x138) + 0xD34);
    *(short *)(*(int *)(self + 0x138) + 0x9C0) = base + 1;
    *(short *)(*(int *)(self + 0x138) + 0x9C2) = 9;
    off = off + 0xD;
    *(short *)(*(int *)(self + 0x138) + 0x9C4) = off;
    *(short *)(*(int *)(self + 0x138) + 0x9C6) = 4;
    *(short *)(*(int *)(self + 0x138) + 0x9C8) = base;
    *(short *)(*(int *)(self + 0x138) + 0x9CA) = 9;
    *(short *)(*(int *)(self + 0x138) + 0x9CC) = off;
    *(short *)(*(int *)(self + 0x138) + 0x9CE) = 5;
    *(short *)(*(int *)(self + 0x138) + 0x9D0) = base + 3;
    *(short *)(*(int *)(self + 0x138) + 0x9D2) = 0x12;
    *(short *)(*(int *)(self + 0x138) + 0x9D4) = off;
    *(short *)(*(int *)(self + 0x138) + 0x9D6) = 2;
    *(short *)(*(int *)(self + 0x138) + 0x9D8) = base + 2;
    *(short *)(*(int *)(self + 0x138) + 0x9DA) = 0x12;
    *(short *)(*(int *)(self + 0x138) + 0x9DC) = off;
    *(short *)(*(int *)(self + 0x138) + 0x9DE) = 3;
    func_08AB8768(self, *(int *)(self + 0x138) + 0x9E0);
    o = *(int *)(self + 0x138);
    func_088D428C(o + 0x400, o + 0x900, o + 0x9C0, 1);
}
