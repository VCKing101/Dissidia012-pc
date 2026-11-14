typedef struct FxLink { int target; unsigned char active; char pad_05[3]; int id; } FxLink;
extern void func_0892EB10(FxLink *l);
extern void func_0892EA3C(FxLink *l);
void func_0892E708(FxLink *l)
{
    l->target = 0;
    l->active = 0;
    l->id = -1;
    func_0892EB10(l);
    func_0892EA3C(l);
}
