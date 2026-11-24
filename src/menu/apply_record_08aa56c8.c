/* Looks up the owner's record, takes its two values (replaced by the table short when the flag bit is set) and applies them to the owner's controls. */
extern int D_089D712C;
extern int D_089D8140;
extern unsigned short D_08AA7EE0[];
extern int func_088CB164(int base, int id);
extern unsigned int func_088CBAD0(int r);
extern void func_08AB4B4C(int p, int a, int id, int b);
extern void func_08AB51F4(int p, int a, int v);
void func_08AA56C8(int self) {
    int r;
    unsigned int v;
    unsigned int w;
    r = func_088CB164(D_089D712C + 0xF58, *(int *)(*(int *)(self + 0x1EE4) + 0x590));
    v = func_088CBAD0(r);
    w = *(unsigned char *)(r + 1);
    if ((*(unsigned *)(D_089D8140 + 0x1C) & 1) != 0) {
        v = D_08AA7EE0[*(int *)(D_089D8140 + 8)];
        w = v;
    }
    func_08AB4B4C(*(int *)(*(int *)(self + 0x1EE4) + 0x1E74), 0, *(int *)(*(int *)(self + 0x1EE4) + 0x590), 0);
    func_08AB51F4(*(int *)(*(int *)(self + 0x1EE4) + 0x1E74), 0, v);
    *(unsigned *)(*(int *)(self + 0x1EE4) + 0x1E48) = v;
    *(unsigned *)(*(int *)(self + 0x1EE4) + 0x1E54) = w;
}
