/* Menu row action: resolves the cursor row's target value and runs or cancels the page effect. */
typedef struct Bk { char p0[0x7C]; int h7C; char p1[0x258 - 0x80]; int h258; char p2[0x2B4 - 0x25C]; int cur; int pad; int sel[1]; char p3[0x308 - 0x2C0]; int pl[1]; } Bk;
typedef struct R { char p[8]; Bk *b; } R;
extern void func_0887B944(int a);
extern void func_088D9CD8(int a);
extern void func_088D9CFC(int a);
extern void func_08A8DB4C(R *s, int a, int b, int c);
extern void func_08A8DF30(R *s, int a);
typedef struct T8 { short a; short v; int pad; } T8;
extern T8 D_08A93A32[];
extern T8 D_08A93A7A[];
void func_08A8CD90(R *self, unsigned char mode, int val)
{
    int v[2];
    Bk *b = self->b;
    int pl = b->pl[b->cur];
    int h;
    if (val != -1) {
        v[1] = val;
        h = b->h7C;
    } else {
        int row = *(int *)((char *)b + pl * 0x2CC + b->sel[pl] * 4 + 0x628);
        if (pl == 0) {
            v[1] = D_08A93A32[row].v;
        } else {
            v[1] = D_08A93A7A[row].v;
        }
        h = b->h7C;
    }
    if (mode == 1) {
        func_088D9CD8(h);
        func_08A8DF30(self, 0xB);
        func_08A8DB4C(self, 0xB, v[1], -1);
    } else {
        func_088D9CFC(h);
        func_0887B944(self->b->h258);
    }
}
