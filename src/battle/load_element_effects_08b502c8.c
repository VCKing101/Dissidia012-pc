// Registers the element piece image and loads the cross, dust and tail effect handles.
struct Res { int pad; int a; int b; };
struct Mgr { char pad[0x174]; int tbl[1]; };
extern struct Res *func_08869AB4(void *, const char *);
extern int func_08B52F88(int, int);
extern void func_08856278(void *, int, int, int);
extern struct Mgr *D_089D4DC8;
extern const char D_08B5180C[], D_08B5181C[], D_08B5182C[], D_08B5183C[];

void func_08B502C8(int *self, void *set, int arg)
{
    struct Res *r;
    r = func_08869AB4(set, D_08B5180C);
    if (r != 0) {
        func_08856278(D_089D4DC8->tbl, arg, r->a, r->b);
    }
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;
    r = func_08869AB4(set, D_08B5181C);
    if (r != 0) {
        self[3] = func_08B52F88(r->a, r->b);
    }
    r = func_08869AB4(set, D_08B5182C);
    if (r != 0) {
        self[4] = func_08B52F88(r->a, r->b);
    }
    r = func_08869AB4(set, D_08B5183C);
    if (r != 0) {
        self[5] = func_08B52F88(r->a, r->b);
    }
}
