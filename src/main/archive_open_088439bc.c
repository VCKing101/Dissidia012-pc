typedef struct Obj088439BC { char pad_00[9]; unsigned char count; char pad_0A[0x1A]; char name[4]; } Obj088439BC;
typedef struct Ctx088439BC { Obj088439BC *obj; } Ctx088439BC;
typedef struct Manager088439BC { char pad_00[0x61]; unsigned char failed; } Manager088439BC;
extern Manager088439BC *D_089D4960;
extern char D_089C88A8[];
extern void func_08843BE0(void);
extern int func_08844FA4(void *name, const char *ext);
extern int func_088FE7C8(int h, unsigned char any);
extern void func_088FE850(int h, int a, int b, void (*cb)(void), Ctx088439BC *c, unsigned char any);
int func_088439BC(Ctx088439BC *c, int a, int b)
{
    int h = func_08844FA4(c->obj->name, D_089C88A8);
    int opened = func_088FE7C8(h, c->obj->count > 0);
    int one = 1;
    if (opened <= 0) {
        D_089D4960->failed = one;
        return 0;
    }
    func_088FE850(h, a, b, func_08843BE0, c, c->obj->count > 0);
    return one;
}
