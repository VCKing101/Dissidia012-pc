/* Frees the entries held by each item of kind 1 or 5 found by walking an item list. */
typedef struct Item { char pad0[6]; short kind; char pad1[0x1C - 8]; int ref; } Item;
typedef struct Def { char pad[2]; unsigned char k; } Def;
extern Item *func_089121A8(int, Item *);
extern Def *func_088BBD8C(int);
extern short *func_08847108(int, int);
extern int func_088444D4(int, int);
extern void func_08844520(int, int);
extern int D_089D4960;
void func_08912304(int owner)
{
    Item *it = 0;
    while ((it = func_089121A8(owner, it)) != 0) {
        if (it->kind == 1) {
            Def *d = func_088BBD8C(it->ref);
            short *s = func_08847108(d->k, 0);
            int x = *s;
            if (func_088444D4(D_089D4960, x) != 0) {
                int y = *s;
                func_08844520(D_089D4960, y);
            }
        } else if (it->kind == 5) {
            { int z = *(short *)((char *)it + 0x1E); func_08844520(D_089D4960, z); }
        }
    }
}
