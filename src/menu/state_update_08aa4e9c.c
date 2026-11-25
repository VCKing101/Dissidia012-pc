/* Menu state update: when both flags are set and the lookup is clear, copies three bytes to the object and refreshes it. */
typedef struct { char p[0x3C]; int f3C; char q[0xF0 - 0x40]; int fF0; char r[0x104 - 0xF4]; int f104; char s[0x118 - 0x108]; int f118; } O;
typedef struct {
    char p0[0x710]; signed char c710; char p1[3]; signed char c714; signed char c715;
    char p2[0x1E58 - 0x716]; unsigned char b1E58;
    char p3[0x1E70 - 0x1E59]; unsigned int f1E70; int f1E74;
    char p4[0x1E98 - 0x1E78]; int f1E98;
} B;
typedef struct { char p[0x1EE4]; B *b; } S;
extern void func_08AA4660(S *);
extern void func_08AA4DDC(S *);
extern O *func_08AA58C4(void);
extern void func_08AB1610(void *);
extern void func_08AB16A8(void *);
extern void func_08AB1748(void *);
extern int func_08AB48F4(int);
extern void func_08AB4CD0(int, int, int, int, int);
extern void func_08AB6AD0(int, int, int);
void func_08AA4E9C(S *self) {
    int v[4];
    int k;
    O *o;
    unsigned int f = self->b->f1E70;
    if ((f & 1) != 0) {
        if ((f & 0x200) != 0) {
            if (func_08AB48F4(self->b->f1E74) == 0) {
                o = func_08AA58C4();
                func_08AB1748((char *)self->b + 0x710);
                func_08AB16A8((char *)self->b + 0x710);
                func_08AB1610((char *)self->b + 0x710);
                v[0] = self->b->c710;
                v[1] = self->b->c715;
                v[2] = self->b->c714;
                o->fF0 = v[0];
                o->f118 = v[1];
                o->f104 = v[2];
                func_08AB4CD0(self->b->f1E74, 0, v[0], v[2], v[1]);
                k = 0;
                func_08AA4660(self);
                if (self->b->b1E58 == 1) {
                    k = 1;
                }
                if (o->f3C != k) {
                    func_08AB6AD0(self->b->f1E74, 0, k);
                }
                o->f3C = k;
                self->b->f1E98 = self->b->f1E98 + 1;
                if (self->b->f1E98 >= 3) {
                    self->b->f1E98 = 3;
                }
                func_08AA4DDC(self);
            }
        }
    }
}
