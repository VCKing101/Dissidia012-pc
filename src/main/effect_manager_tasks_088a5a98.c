extern void *func_088823C4(int p1, int p2, int p3, void (*fn)(void), char *name);
extern void func_088A5C34(void);
extern void func_088A5C54(void);
extern void func_088A5C74(void);
extern void func_088A5C94(void);
extern void func_088A5CB4(void);
extern void func_088A5CD4(void);
extern void func_088A5D18(void);
extern void func_088A5D38(void);
extern void func_088A5D58(void);
extern char D_089CAF78[];
extern char D_089CAF94[];
extern char D_089CAFB0[];
extern char D_089CAFCC[];
extern char D_089CAFE8[];
extern char D_089CB004[];
extern char D_089CB020[];
extern char D_089CB03C[];
extern char D_089CB05C[];

void func_088A5A98(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int s0, int s4, int s8) {
    func_088823C4(p1, p2, p4, func_088A5C34, D_089CAF78);
    func_088823C4(p1, p2, p5, func_088A5C54, D_089CAF94);
    func_088823C4(p1, p2, p5 + 1, func_088A5D18, D_089CAFB0);
    func_088823C4(p1, p3, p6, func_088A5C74, D_089CAFCC);
    func_088823C4(p1, p3, p7, func_088A5C94, D_089CAFE8);
    func_088823C4(p1, p3, p8, func_088A5CB4, D_089CB004);
    func_088823C4(p1, p3, s0, func_088A5CD4, D_089CB020);
    func_088823C4(p1, p3, s4, func_088A5D38, D_089CB03C);
    func_088823C4(p1, p3, s8, func_088A5D58, D_089CB05C);
}
