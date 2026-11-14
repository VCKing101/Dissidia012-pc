typedef struct FxRes { int state; char list[8]; int model; int anim; } FxRes;
extern char D_089CED80[];
extern void func_0892CB5C(FxRes *res);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern int func_0887E534(int model);
extern void func_088D428C(void *list, int *models, const char *name, int count);
void func_0892CAB8(FxRes *res, int a, int b, int d, int e, int f)
{
    int model;
    func_0892CB5C(res);
    model = func_0887B3EC(a, b, 0, d, e, f);
    res->model = model;
    res->anim = func_0887E534(model);
    func_088D428C(res->list, &res->model, D_089CED80, 1);
}
