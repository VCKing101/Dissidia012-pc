/* Menu toggle: when the lookup is -1, sets or clears the 0x100 flag according to a second query. */
typedef struct {
    char p0[0x8044]; int f8044;
    char p1[0x81B4 - 0x8048]; unsigned int f81B4;
} B;
typedef struct { char p[0x154]; B *b; char q[0x894 - 0x158]; char a894[1]; } S;
extern int func_08A9C814(S *);
extern void func_08AAB7D0(int, int, int);
extern int func_08AB47BC(void *);
extern void func_08AB5790(void *, int);
extern int func_08AB6BEC(void *, int, int);
void func_08A9C83C(S *self) {
    void *s3 = self->a894;
    int one;
    unsigned int fl;
    if (func_08AB47BC(s3) != -1) {
        return;
    } else {
        one = 1;
        if (func_08AB6BEC(s3, 0x10, 1) == one) {
            fl = self->b->f81B4;
            if ((fl & 0x1000) == 0) {
                if (func_08A9C814(self) != one) {
                    if ((self->b->f81B4 & 0x100) == 0) {
                        func_08AB5790(s3, 8);
                        func_08AAB7D0(self->b->f8044, 1, 1);
                        self->b->f81B4 |= 0x100;
                    }
                }
            }
        } else {
            fl = self->b->f81B4;
            if ((fl & 0x100) != 0) {
                func_08AB5790(s3, 9);
                func_08AAB7D0(self->b->f8044, 0, 1);
                self->b->f81B4 &= ~0x100;
            }
        }
    }
}
