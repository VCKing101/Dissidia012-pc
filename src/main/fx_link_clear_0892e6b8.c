typedef struct FxLink { int target; unsigned char active; char pad_05[3]; int id; int a[1]; int b[1]; } FxLink;
void func_0892E6B8(FxLink *l)
{
    int i;
    l->target = 0;
    l->active = 0;
    l->id = -1;
    for (i = 0; i < 1; i++) {
        l->a[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        l->b[i] = 0;
    }
}
