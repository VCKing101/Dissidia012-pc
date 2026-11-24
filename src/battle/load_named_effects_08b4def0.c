// Clears four effect handles then loads the four named effects from the resource set.
struct Res { int pad; int a; int b; };
extern struct Res *func_08869AB4(void *, const char *);
extern int func_08B52F88(int, int);
extern const char D_08B4EC48[], D_08B4EC58[], D_08B4EC6C[], D_08B4EC7C[];

void func_08B4DEF0(int *self, void *set)
{
    int i;
    int *p;
    struct Res *r;
    for (i = 0, p = self; i < 4; i++, p++) {
        p[3] = 0;
    }
    r = func_08869AB4(set, D_08B4EC48);
    if (r != 0) {
        self[3] = func_08B52F88(r->a, r->b);
    }
    r = func_08869AB4(set, D_08B4EC58);
    if (r != 0) {
        self[4] = func_08B52F88(r->a, r->b);
    }
    r = func_08869AB4(set, D_08B4EC6C);
    if (r != 0) {
        self[5] = func_08B52F88(r->a, r->b);
    }
    r = func_08869AB4(set, D_08B4EC7C);
    if (r != 0) {
        self[6] = func_08B52F88(r->a, r->b);
    }
}
