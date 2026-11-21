extern int D_089D5FBC;                 /* the "MS Write" thread, -1 = none */
extern char D_089C9674[];              /* "MS Write" */
extern int func_0886FF1C(int args, void *argp);                 /* the thread's entry */
extern int func_089C5FE8(int thread);
extern int func_089C5FA8(int thread, int unk);
extern int func_089C5FD0(const char *name, int (*entry)(int, void *), int priority, int stack, int attr, void *option);
extern int func_089C5FA0(int thread, int args, void *argp);
extern int func_089C5FF8(int a, int b);

/* (Re)start the "MS Write" thread: stop and delete the old one first. */
int func_08870154(void)
{
    if (D_089D5FBC >= 0) {
        func_089C5FE8(D_089D5FBC);
        if (func_089C5FA8(D_089D5FBC, 0) != 0) {
            return 0x80000017;
        }
        D_089D5FBC = -1;
    }
    D_089D5FBC = func_089C5FD0(D_089C9674, func_0886FF1C, 0x21, 0x1000, 0x4000, 0);
    func_089C5FA0(D_089D5FBC, 0, 0);
    func_089C5FF8(0, 0);
    return 1;
}
