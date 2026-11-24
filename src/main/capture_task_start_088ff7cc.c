/* Starts the capture: func_088FE9E8 if func_088FEA24 says so, func_088FEA68, then registers the "ExecCaptureTask" task (func_0890009C, size 0x1B198) and sets the D_089D8134/8138/812C state. */
extern int D_089D69A8;
extern int D_089D8134;
extern char D_089D8138;
extern int D_089D812C;
extern char D_089CDA50[];
extern int func_088FEA24(void);
extern void func_088FE9E8(void);
extern void func_088FEA68(void);
extern void func_0890009C(void);
extern void *func_08882274(int v);
extern void func_088823C4(void *w, int a, int size, void (*fn)(void), char *name);
void func_088FF7CC(void)
{
    if (func_088FEA24() != 0) {
        func_088FE9E8();
    }
    func_088FEA68();
    func_088823C4(func_08882274(D_089D69A8), -1, 0x1B198, func_0890009C, D_089CDA50);
    D_089D8134 = 0;
    D_089D8138 = 0;
    D_089D812C = 1;
}
