extern "C" {
typedef struct FxHolder { char pad_00[0x30]; void *models[5]; } FxHolder;
extern void func_088D9B28(void *p, unsigned char on);
void func_0892D88C(FxHolder *h, unsigned char on)
{
    for (int i = 0; i < 5; i++) {
        if (h->models[i] == 0) {
            continue;
        }
        func_088D9B28(h->models[i], on);
    }
}
}
