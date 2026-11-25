/* For each of the five entries (0x16DC bytes each from +0xA58 of the block) stores func_08AB1C24 of it in the block's list at +0x38 of the object at +0x7CCC, then reads three values of each entry and passes them with the entry index to func_08AB6D04. */
typedef struct Ent08AAAB84 { char pad[0x16DC]; } Ent08AAAB84;
typedef struct List08AAAB84 { char pad[0x38]; int w38[5]; } List08AAAB84;
typedef struct Blk08AAAB84 {
    char pad0[0xA58];
    Ent08AAAB84 ents[5];
    char pad1[0x7CCC - 0xA58 - 5 * 0x16DC];
    List08AAAB84 *list;
} Blk08AAAB84;
typedef struct S08AAAB84 { char pad[0x7CFC]; Blk08AAAB84 *blk; } S08AAAB84;
extern int func_08AB1C24(Ent08AAAB84 *e);
extern int func_08AB1748(Ent08AAAB84 *e);
extern int func_08AB1610(Ent08AAAB84 *e);
extern int func_08AB16A8(Ent08AAAB84 *e);
extern void func_08AB6D04(List08AAAB84 *l, int i, int a, int b, int c, int n);
void func_08AAAB84(S08AAAB84 *self)
{
    int t[4];
    int i;
    for (i = 0; i < 5; i++) {
        int v = func_08AB1C24(&self->blk->ents[i]);
        self->blk->list->w38[i] = v;
    }
    for (i = 0; i < 5; i++) {
        Ent08AAAB84 *e = &self->blk->ents[i];
        t[0] = func_08AB1748(e);
        t[1] = func_08AB1610(e);
        t[2] = func_08AB16A8(e);
        func_08AB6D04(self->blk->list, i, t[0], t[1], t[2], 5);
    }
}
