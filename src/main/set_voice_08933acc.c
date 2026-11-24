/* Stops the current mode, switches to the new one, builds its table into a local buffer, stores the resulting handle and starts it. */
typedef struct Voice { signed char mode; char pad[3]; int handle; } Voice;
extern void (*D_089D85D0[])(void);
extern void func_08933B4C(Voice *p);
extern void func_08933A80(Voice *p, int mode, char *buf, int n);
extern int func_089331FC(char *buf, int z);
void func_08933ACC(Voice *p, signed char mode)
{
    char buf[256];
    func_08933B4C(p);
    p->mode = mode;
    func_08933A80(p, p->mode, buf, 0x100);
    p->handle = func_089331FC(buf, 0);
    D_089D85D0[p->mode]();
}
