extern "C" {
typedef struct FxRes { int state; char list[8]; int models[2]; } FxRes;
extern void func_0887BA5C(int model);
void func_0892CB5C(FxRes *res)
{
    for (int i = 0; i < 2; i++) {
        if (res->models[i] != 0) {
            func_0887BA5C(res->models[i]);
            res->models[i] = 0;
        }
    }
}
}
