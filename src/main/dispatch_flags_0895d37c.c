// Stores the flag at +0x50 as 0/1 into the global's sub-object, then runs the handler chosen by the flags at +0x4E, +0x4D, +0x4C.
struct P { char pad[0x40]; unsigned v; };
struct G { char pad[0xAC]; struct P *p; };
struct S { char pad[0x4C]; unsigned char f4c, f4d, f4e, f4f, f50; };
extern struct G *D_09B6F478;
extern void func_0895D668(int);
extern void func_0895E108(int, struct S *);
extern void func_0895D40C(int, struct S *);
void func_0895D37C(int a, struct S *s) {
    D_09B6F478->p->v = (s->f50 != 0);
    if (s->f4e != 0) {
        func_0895D40C(a, s);
    } else if (s->f4d != 0) {
        func_0895E108(a, s);
    } else if (s->f4c != 0) {
        func_0895D668(a);
    }
}
