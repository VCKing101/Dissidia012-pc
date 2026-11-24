/* If bit 0 of the word at +0x1C of D_089D8140 is set calls func_08900CBC(obj, b, c) and returns b; else returns func_088CB164(work+0xF58, c). */
struct Obj { char pad[0x1C]; unsigned flags; };
extern struct Obj *D_089D8140;
extern char *D_089D712C;
extern void func_08900CBC(struct Obj *o, int b, int c);
extern int func_088CB164(char *p, int c);
int func_08B3079C(int a, int b, int c)
{
    if (D_089D8140->flags & 1) {
        func_08900CBC(D_089D8140, b, c);
        return b;
    } else {
        return func_088CB164(D_089D712C + 0xF58, c);
    }
}
