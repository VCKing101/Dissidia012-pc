typedef struct Stats08841B84 { char pad_00[0xC]; struct { int lo; int hi; } totals[8]; int count; } Stats08841B84;
extern void *D_089D4960;
extern void func_088135D4(void *item);
extern void func_08847158(void *item);
extern void func_08844068(void *manager);
extern void func_08842308(void);
void func_08841B84(Stats08841B84 *s, void *item)
{
    if (item == 0) {
        return;
    }
    s->totals[0].hi = 0;
    s->totals[0].lo = 0;
    s->totals[1].hi = 0;
    s->totals[1].lo = 0;
    s->totals[2].hi = 0;
    s->totals[2].lo = 0;
    s->totals[3].hi = 0;
    s->totals[3].lo = 0;
    s->totals[4].hi = 0;
    s->totals[4].lo = 0;
    s->totals[5].hi = 0;
    s->totals[5].lo = 0;
    s->totals[6].hi = 0;
    s->totals[6].lo = 0;
    s->totals[7].hi = 0;
    s->totals[7].lo = 0;
    s->count = 0;
    func_088135D4(item);
    func_08847158(item);
    func_08844068(D_089D4960);
    func_08842308();
}
