typedef struct Rot { float x, y, z, w; int cur; unsigned char flag; } Rot;
typedef struct Src { char pad0[0x16]; unsigned char b; char pad2; unsigned char c; } Src;

extern void func_0895477C(int a, Rot *r, Src *s, Rot *r2);
extern int func_0895470C(int a, char *p);
extern void func_08954918(Src *s, Rot *r, int v);
extern void func_089549C8(int a, Src *s, Rot *r);

int func_089545B8(int unused, int a, Src *s, Rot *r) {
    Src *sr = s;
    Rot *o = r;
    int ar = a;
    o->cur = 0;
    o->flag = 1;
    if (sr->c != 0) {
        o->x = 0.0f;
        o->y = 0.0f;
        o->z = 0.0f;
        o->w = 1.0f;
    } else if (sr->b == 0) {
        func_0895477C(ar, o, sr, o);
    } else {
        int v = func_0895470C(ar, (char *)sr + 0x10);
        func_08954918(sr, o, v);
    }
    func_089549C8(ar, sr, o);
    return 1;
}
