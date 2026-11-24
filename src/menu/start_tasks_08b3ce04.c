/* Allocates the task pool, registers the owner's main and draw tasks and marks the owner active. */
extern void *func_088820D8(int size);
extern void func_0886504C(void *a, void *b, int size);
extern void func_08B3D9FC(void *owner);
extern void func_08865088(void *a, int b, int c, void *fn, const char *name);
extern void func_08B3CF00(void);
extern void func_08B3CF20(void);
extern const char D_08B418F4[];
extern const char D_08B41900[];

typedef struct O3CE04 { char pad[0x58]; void *pool; char pad5C[0x28]; int f84; int f88; } O3CE04;
extern O3CE04 *D_08B41CC0;

void func_08B3CE04(void)
{
    void *pool = func_088820D8(0x2000);
    D_08B41CC0->pool = pool;
    func_0886504C((char *)D_08B41CC0 + 0x5C, D_08B41CC0->pool, 0x2000);
    D_08B41CC0->f84 = 1;
    D_08B41CC0->f88 = 1;
    func_08B3D9FC(D_08B41CC0);
    func_08865088((char *)D_08B41CC0 + 0x5C, -1, 0x4B0, (void *)func_08B3CF00, D_08B418F4);
    func_08865088((char *)D_08B41CC0 + 0x5C, -1, 110000, (void *)func_08B3CF20, D_08B41900);
}
