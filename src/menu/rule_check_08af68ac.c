typedef struct V { int a; int b; } V;
typedef struct R { char pad[0xE]; signed char lv; } R;
extern void func_088B97E8(V *, int);
extern int func_08AF6428(int, V *, int, int);
extern unsigned int func_08AF8F84(int, V *, int);
int func_08AF68AC(int h, unsigned int *kind, int p3, R *other, int cmp, unsigned short mask, unsigned short id, unsigned char flag)
{
    int hit;
    V v;
    V tmp;
    int r;
    hit = 0;
    func_088B97E8(&tmp, id);
    v.a = tmp.a;
    v.b = tmp.b;
    if (other != 0) {
        r = func_08AF6428(h, &v, p3, 0);
        switch (*kind) {
        case 0: {
            unsigned int u = func_08AF8F84(h, &v, 1);
            if (u != 0) {
                if (mask == 0) {
                    hit = 1;
                } else if ((u & 0x100) != 0 && (mask & 0x200) != 0) {
                    hit = 1;
                }
            }
            break;
        }
        case 1:
            {
                R *t = (R *)v.a;
                if (t != 0 && t->lv < other->lv) {
                    hit = 1;
                }
            }
            break;
        case 2:
            {
                R *t = (R *)v.a;
                if (t != 0 && other->lv < t->lv) {
                    hit = 1;
                }
            }
            break;
        case 3:
            if (r < cmp) {
                hit = 1;
            }
            break;
        case 4:
            if (cmp < r) {
                hit = 1;
            }
            break;
        case 5:
            if (flag == 0) {
                hit = 1;
            }
            break;
        }
    }
    return hit;
}
