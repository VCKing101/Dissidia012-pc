extern "C" {
typedef struct FxSet { int state; void *models[2]; } FxSet;
extern void func_088DA060(void *p);
void func_0892CA54(FxSet *s)
{
    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            continue;
        }
        if (s->models[i] != 0) {
            func_088DA060(s->models[i]);
            s->models[i] = 0;
        }
    }
}
}
