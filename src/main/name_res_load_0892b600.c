typedef struct NameRes { char pad_00[8]; char list[0x1C]; int model0; int anim0; int model1; int anim1; } NameRes;
extern char D_089CED1C[];
extern char D_089CECA0[];
extern void func_0892B7A4(NameRes *res);
extern void func_089347E8(char *dst, const char *fmt, int id);
extern int func_0887B3EC(int a, int b, int c, int d, char **names, int count);
extern int func_0887E534(int model);
extern void func_088D428C(void *list, int *models, const char *name, int count);
void func_0892B600(NameRes *res, int a, int b, int id0, int id1)
{
    char name[64];
    char *names;
    int model;
    func_0892B7A4(res);
    names = name;
    func_089347E8(name, D_089CED1C, id0);
    model = func_0887B3EC(a, b, 0, 0, &names, 1);
    res->model0 = model;
    res->anim0 = func_0887E534(model);
    func_089347E8(name, D_089CED1C, id1);
    model = func_0887B3EC(a, b, 0, 1, &names, 1);
    res->model1 = model;
    res->anim1 = func_0887E534(model);
    func_088D428C(res->list, &res->model0, D_089CECA0, 1);
}
