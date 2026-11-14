extern "C" {
typedef struct FxHolder { char pad_00[0x30]; void *models[1]; } FxHolder;
extern void func_088DA060(void *p);
void func_0892CFF8(FxHolder *h)
{
    for (int i = 0; i < 1; i++) {
        if (h->models[i] != 0) {
            func_088DA060(h->models[i]);
            h->models[i] = 0;
        }
    }
}
}
