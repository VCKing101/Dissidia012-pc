/* Starts a timed effect for each item of kind 1 (when not yet running) or 5 found by walking an item list. */
typedef struct Item { char pad0[6]; short kind; char pad1[0x1C - 8]; int ref; } Item;
typedef struct Def { char pad[2]; unsigned char k; } Def;
extern Item *func_089121A8(int, Item *);
extern Def *func_088BBD8C(int);
extern short *func_08847108(int, int);
extern int func_088444D4(int, int);
extern int func_089110B4(void);
extern void func_0884417C(int, int, int, int, int);
extern int D_089D4960;
void func_089121F4(int owner)
{
    Item *it = 0;
    while ((it = func_089121A8(owner, it)) != 0) {
        if (it->kind == 1) {
            Def *d = func_088BBD8C(it->ref);
            short *s = func_08847108(d->k, 0);
            int x = *s;
            if (func_088444D4(D_089D4960, x) == 0) {
                short v = *s;
                int t = func_089110B4();
                func_0884417C(D_089D4960, v, t, 0x264000, 0);
            }
        } else if (it->kind == 5) {
            short v = *(short *)((char *)it + 0x1E);
            int t = func_089110B4();
            func_0884417C(D_089D4960, v, t, 0x264000, 0);
        }
    }
}
