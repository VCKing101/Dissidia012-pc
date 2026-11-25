/* Menu page layout: loads one of two layouts (flag at 0xF28) with its icons, then refreshes. */
typedef struct { char p[0xE]; short h0E; } W;
typedef struct {
    char p0[0x400]; char a400[1];
    char p1[0x520 - 0x401]; W *w520; W *w524;
    char p2[0x880 - 0x528]; char a880[1];
    char p3[0xF28 - 0x881]; int fF28;
    char p4[0xF50 - 0xF2C]; char aF50[1];
} B;
typedef struct { char p[0xDB30]; B *b; } S;
extern char D_08AD96BC[], D_08AD96D4[], D_08AD972C[], D_08AD97A4[], D_08AD97BC[], D_08AD9824[];
extern void func_088D3CF0(void *, void *, void *);
extern void func_088D428C(void *, void *, void *, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_08ACA7B8(S *);
void func_08ACA8E8(S *self) {
    if (self->b->fF28 == 0) {
        func_088D46E4(self->b->aF50, 0, 0, 1, 0, -1);
        func_088D3CF0(self->b, self->b->a400, D_08AD96BC);
        func_088D3CF0(self->b, self->b->a400, D_08AD96D4);
        self->b->w520->h0E = 0x25;
        {
            short c;
            c = 0x26;
            self->b->w524->h0E = c;
        }
        func_088D428C(self->b->a400, self->b->a880, D_08AD972C, 1);
    } else {
        func_088D46E4(self->b->aF50, 0, 0, 2, 0, -1);
        func_088D3CF0(self->b, self->b->a400, D_08AD97A4);
        func_088D3CF0(self->b, self->b->a400, D_08AD97BC);
        self->b->w520->h0E = 0x2B;
        {
            short c;
            c = 0x2C;
            self->b->w524->h0E = c;
        }
        func_088D428C(self->b->a400, self->b->a880, D_08AD9824, 1);
    }
    func_08ACA7B8(self);
}
