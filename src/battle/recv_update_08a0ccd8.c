struct Rec { char pad[8]; float f8; char pad2[0x18]; unsigned char c24; char t[7]; };
struct O { char pad[8]; int state; char p1[8]; char a14[0x28]; char a3c[0x30]; char a6c[0x2C]; int h98; int h9c; int ha0; };
extern int func_08A22E2C(struct Rec *r, int a, int b, int c);
extern int func_08A10F74(void *p);
extern void func_08A10DB0(void *p);
extern void func_08A10EA0(void *p);
extern float func_08891348(int a);
extern void func_08A16CC4(float f, void *p);
extern void func_08A16C58(void *p);
extern void func_08A01C74(void *p, int a);
int func_08A0CCD8(struct O *s)
{
    struct Rec r;
    char *q;
    if (func_08A22E2C(&r, s->h98, s->h9c, 0x1B) == 0) {
        int one = 1;
        q = s->a6c;
        if (r.c24 == one) {
            if (func_08A10F74(q) == one) { func_08A10DB0(q); }
        } else {
            if (func_08A10F74(q) == 0) { func_08A10EA0(q); }
        }
        {
            float d = func_08891348(s->ha0);
            if (d != 0.0f) { func_08A16CC4(r.f8 / d, s->a3c); }
        }
    } else {
        func_08A01C74(s->a14, 2);
        func_08A16C58(s->a3c);
        if (func_08A10F74(s->a6c) == 0) { func_08A10EA0(s->a6c); }
        s->state = 3;
    }
    return 0;
}
