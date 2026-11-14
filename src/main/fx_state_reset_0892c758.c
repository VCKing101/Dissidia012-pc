typedef struct ShortQuad { unsigned short v[4]; } ShortQuad;
typedef struct FxState { int state; int a[2]; int b[2]; ShortQuad ids[2]; int timer; } FxState;
void func_0892C758(FxState *fx)
{
    int i;
    fx->state = 0;
    for (i = 0; i < 2; i++) {
        fx->a[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        fx->b[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        fx->ids[i].v[0] = 0xFFFF;
        fx->ids[i].v[1] = 0xFFFF;
        fx->ids[i].v[2] = 0xFFFF;
        fx->ids[i].v[3] = 0xFFFF;
    }
    fx->timer = 0;
}
