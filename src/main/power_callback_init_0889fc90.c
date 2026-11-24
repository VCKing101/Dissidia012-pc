/* Creates the music semaphore and registers the power callback. */
extern int D_089D6C38;
extern int D_089D6C58;
extern char D_089CAB80[];

extern int func_08882118(int attr, int count);
extern int func_089C5F80(const char *name, int (*fn)(void), void *arg);
extern void func_089C57F8(int slot, int callback);
extern int func_088A0508(void);

void func_0889FC90(void)
{
    D_089D6C38 = func_08882118(0x18002, 0x80);
    D_089D6C58 = func_089C5F80(D_089CAB80, func_088A0508, 0);
    func_089C57F8(4, D_089D6C58);
}
