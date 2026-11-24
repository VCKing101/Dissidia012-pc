/* Returns 1 for ids outside 1..0x22 or equal to 0x17; otherwise the byte from the global's value table when its flag bit is set, else the lookup result. */
extern int D_089D712C;
extern int D_089D8140;
extern unsigned short D_08AB2478[];
extern int func_088CB164(int base, int id);
extern unsigned int func_088CBAD0(int r);
unsigned int func_08AB1CDC(int id) {
    unsigned int res;
    if (id <= 0 || id >= 0x23 || id == 0x17) {
        return 1;
    }
    res = func_088CBAD0(func_088CB164(D_089D712C + 0xF58, id));
    if ((*(unsigned *)(D_089D8140 + 0x1C) & 1) != 0) {
        res = D_08AB2478[*(int *)(D_089D8140 + 8)];
    }
    return res;
}
