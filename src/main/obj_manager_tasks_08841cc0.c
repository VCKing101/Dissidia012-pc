typedef struct Task102 { char pad[0x30]; void (*fn30)(void); } Task102;
extern int func_0888232C(int a, char *name);
extern Task102 *func_088823C4(int p1, int p2, int p3, void (*fn)(void), char *name);
extern void func_08808424(int a, int b, int c);
extern int *D_089D4958;
extern void func_088413D0(void);
extern void func_088416EC(void);
extern void func_08841798(void);
extern void func_088417C8(void);
extern void func_088417F8(void);
extern void func_08841950(void);
extern void func_0884197C(void);
extern void func_08841998(void);
extern void func_088419B4(void);
extern void func_08841680(void);
extern void func_088419D0(void);
extern void func_088419EC(void);
extern char D_089C86A0[], D_089C86A8[], D_089C86C0[], D_089C86DC[], D_089C86F8[], D_089C8710[], D_089C872C[], D_089C8750[], D_089C876C[], D_089C8788[];

void func_08841CC0(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8,
                   int x0, int x4, int x8, int xc, int p13, int p14, int p15, int p16,
                   int p17, int p18, int p19) {
    Task102 *t;
    *D_089D4958 = func_0888232C(D_089D4958[2], D_089C86A0);
    t = func_088823C4(*D_089D4958, 0, 0, func_088413D0, D_089C86A8);
    t->fn30 = func_088416EC;
    t = func_088823C4(p1, p2, p3, func_08841798, D_089C86C0);
    t->fn30 = func_088417C8;
    t = func_088823C4(p1, p4, p5, func_088417F8, D_089C86DC);
    t->fn30 = func_08841950;
    func_088823C4(p1, p6, p7, func_0884197C, D_089C86F8);
    func_088823C4(p1, p8, x0, func_08841998, D_089C8710);
    func_088823C4(p1, x4, x8, func_088419B4, D_089C872C);
    func_088823C4(p1, p14, p15, func_08841680, D_089C8750);
    func_088823C4(p1, p16, p17, func_088419D0, D_089C876C);
    func_088823C4(p1, p18, p19, func_088419EC, D_089C8788);
    func_08808424(p1, xc, p13);
}
