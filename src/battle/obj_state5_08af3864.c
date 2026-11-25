// Battle object state 5 -> unlink from the list and finish (state 7); otherwise start a fade effect and enter state 5.
struct O08AF3864 { char p0[0x24]; int f24; char p1[0x34]; int st5c; int pad; int state; char p2[0x14]; char sub[8]; int link; char p3[0x10]; int f98; };
struct G08AF3864 { char pad[0x1660]; void *head; };
extern struct G08AF3864 *D_089D8364;
extern int func_088D4C5C(void);
extern void func_08873D34(int a, void (*fn)(void), void *o, int b, int c, float f);
extern void func_08873DB0(float *v, int n);
extern void func_08907300(void *a, int b);
extern void func_08AF62B8(void);
extern void func_08AF3ABC(void);
void func_08AF3864(struct O08AF3864 *self)
{
    if (self->state == 5) {
        struct G08AF3864 *g = D_089D8364;
        if (g) {
            self->link = (int)g->head;
            g->head = self->sub;
            func_08907300(g, 0);
        }
        if (self->f24) {
            func_08AF62B8();
        }
        self->f98 = 0;
        self->st5c = 7;
    } else {
        float v[4];
        float f = (float)func_088D4C5C(); func_08873D34(3, func_08AF3ABC, self, 0, 2, f);
        v[0] = 255.0f; v[1] = 255.0f; v[2] = 255.0f; v[3] = 255.0f;
        func_08873DB0(v, 2);
        self->st5c = 5;
    }
}
