/* For each of the block's entries (count at +0xD40) with a valid icon id in the 14-byte table, registers it with func_088D4584 at slot 0x21 + index. */
typedef struct Pair08AB94C0 { int v; int w; } Pair08AB94C0;
typedef struct Blk08AB94C0 {
    char pad0[0x400];
    char at400[0x560];
    Pair08AB94C0 pairs[0x7C];
    int count;
    int ids[0x40];
} Blk08AB94C0;
typedef struct S08AB94C0 { char pad[0x138]; Blk08AB94C0 *blk; } S08AB94C0;
typedef struct Icon08AB94C0 { char pad[0xA]; unsigned short icon; char pad2[2]; } Icon08AB94C0;
extern Icon08AB94C0 D_08ABB7FA[];
extern void func_088D4584(void *a, int slot, int icon, int v);
void func_08AB94C0(S08AB94C0 *self)
{
    int i;
    for (i = 0; i < self->blk->count; i++) {
        int icon = D_08ABB7FA[self->blk->ids[i]].icon;
        if (icon != 0xFFFF) {
            func_088D4584((char *)self->blk + 0x400, i + 0x21, icon, self->blk->pairs[i].v);
        }
    }
}
