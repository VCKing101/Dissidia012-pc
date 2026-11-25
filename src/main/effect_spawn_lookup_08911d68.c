extern void *func_088BBD8C(int a);
extern unsigned short D_089CF350[];
extern void func_08911CC8(float f, int a, int b, int c, int d, int e, int g, int h);
typedef struct Ent { char pad[2]; unsigned char idx; unsigned char alt; char pad2; unsigned char k; } Ent;
void func_08911D68(float f, int a, int b, int c, int d, int e) {
    Ent *en = func_088BBD8C(e);
    unsigned short v;
    if (en == 0) { return; }
    if (en->alt == 0) {
        v = D_089CF350[en->idx * 2 + 1];
    } else {
        v = D_089CF350[en->idx * 2];
    }
    func_08911CC8(f, a, b, c, d, 1, v, en->k);
}
