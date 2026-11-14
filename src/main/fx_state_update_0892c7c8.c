typedef struct FxModel { char pad_00[0xC]; short kind; } FxModel;
typedef struct FxState { int state; int pos; FxModel *model; int fC; int f10; short f14[8]; int timer; } FxState;
extern char D_089CED80[];
extern void func_088D3F00(int mode, int *pos, short *ids);
extern void func_088D44EC(int *pos, int *dst, const char *name);
extern void func_088D9F5C(FxModel *model, int kind, int flag);
void func_0892C7C8(FxState *fx)
{
    if (fx->state == 1) {
        func_088D3F00(0, &fx->pos, fx->f14);
        func_088D44EC(&fx->pos, &fx->fC, D_089CED80);
        if (fx->model->kind == 11) {
            if (fx->timer >= 150) {
                func_088D9F5C(fx->model, 12, 1);
            } else {
                fx->timer++;
            }
        }
    }
}
