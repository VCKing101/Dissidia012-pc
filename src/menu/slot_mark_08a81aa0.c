/* Marks one of three slots (or clears the mark for -1): sets the 0x2000 flag, stores the value and notifies. */
typedef struct {
    char p0[0x574]; int w574;
    char p1[0x5E8 - 0x578]; unsigned int f5E8;
    char p2[0x7B8 - 0x5EC]; short h7B8, h7BA, h7BC;
} B;
typedef struct { char p[0x28]; B *b; } S;
extern unsigned short D_08A85BF6;
extern void func_088D9F5C(int, int, int);
extern void func_08A7F96C(S *, int, int, int);
void func_08A81AA0(S *self, int k, int v) {
    int one, two;
    if (k == -1) {
        self->b->f5E8 &= ~0x2000;
    } else if (k == 0) {
        func_088D9F5C(self->b->w574, D_08A85BF6, 1);
        self->b->f5E8 |= 0x2000;
        self->b->h7B8 = v;
        func_08A7F96C(self, 5, 1, 0);
    } else if (k == (one = 1)) {
        self->b->f5E8 |= 0x2000;
        self->b->h7BA = v;
        func_08A7F96C(self, 7, 1, 0);
    } else if (k == (two = 2)) {
        self->b->f5E8 |= 0x2000;
        self->b->h7BC = v;
        func_08A7F96C(self, 6, 1, 0);
    }
}
