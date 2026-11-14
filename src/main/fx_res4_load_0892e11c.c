typedef struct FxRes4 { char pad_00[0x30]; char list[8]; int model; } FxRes4;
extern char D_089CEF48[];
extern void func_0892E1B4(FxRes4 *res);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern void func_088D428C(void *list, int *models, const char *name, int count);
void func_0892E11C(FxRes4 *res, int a, int b, int d, int e, int f)
{
    func_0892E1B4(res);
    res->model = func_0887B3EC(a, b, 0, d, e, f);
    func_088D428C(res->list, &res->model, D_089CEF48, 1);
}
