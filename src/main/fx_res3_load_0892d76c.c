typedef struct FxRes2 { char pad_00[0x30]; char list[0x14]; int f44[2]; int model; int anim; } FxRes2;
extern char D_089CEE88[];
extern char D_089CEE20[];
extern void func_0887BA5C(int model);
extern void func_089347E8(char *dst, const char *fmt, int id);
extern int func_0887B3EC(int a, int b, int c, int d, char **names, int count);
extern int func_0887E534(int model);
extern void func_088D428C(void *list, int *models, const char *name, int count);
void func_0892D76C(FxRes2 *res, int a, int b, int id)
{
    char name[64];
    char *names;
    int model;
    if (res->model != 0) {
        func_0887BA5C(res->model);
        res->model = 0;
    }
    if (res->anim != 0) {
        func_0887BA5C(res->anim);
        res->anim = 0;
    }
    names = name;
    func_089347E8(name, D_089CEE88, id);
    model = func_0887B3EC(a, b, 0, 4, &names, 1);
    res->model = model;
    res->anim = func_0887E534(model);
    func_088D428C(res->list, res->f44, D_089CEE20, 1);
}
