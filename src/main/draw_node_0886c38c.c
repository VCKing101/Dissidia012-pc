typedef struct E { unsigned c0; int c4; unsigned flags; } E;
typedef struct S { char p0[0xC]; int ctx; char p10[0xF8]; E *ents; } S;
extern int func_089B0E3C(int);
extern int func_089B2CCC(int, int, int);
extern void func_089B0D88(int, int, int);
extern void func_089A0C20(unsigned);
extern void func_089A12A4(int, int, int, unsigned, unsigned);
extern void func_089B47AC(int, int, int);
extern void func_0886C38C(S *, int, unsigned char, unsigned, unsigned, unsigned char);

void func_0886C38C(S *self, int idx, unsigned char rec, unsigned alpha, unsigned col, unsigned char mode)
{
    int ctx = self->ctx;
    int off = idx * 12;
    E *e;
    int a, b;
    if (rec != 0) {
        int n = func_089B0E3C(ctx);
        int i;
        ctx = self->ctx;
        for (i = idx; i < n; i++) {
            if (func_089B2CCC(ctx, i, idx) != 0) {
                func_0886C38C(self, i, 0, alpha, col, mode);
            }
            ctx = self->ctx;
        }
    }
    func_089B0D88(ctx, 0x21, 0);
    e = (E *)((char *)self->ents + off);
    if ((e->flags & 0x10000) != 0) {
        func_089A0C20(e->c0);
        alpha = (int)(alpha * (*(unsigned *)((char *)self->ents + off) >> 24)) / 0xFF;
        ctx = self->ctx;
    } else {
        func_089A0C20(col);
        ctx = self->ctx;
    }
    if ((int)alpha < 0xFF && (int)alpha >= 0) {
        unsigned x, y;
        func_089B0D88(ctx, 0x200000, 0);
        x = alpha | alpha << 8 | alpha << 16;
        y = 0xFF - alpha;
        y = y | y << 8 | y << 16;
        if (mode == 0) {
            func_089A12A4(0, 10, 10, x, y);
            ctx = self->ctx;
        } else {
            func_089A12A4(0, 10, 10, x, 0xFFFFFFFF);
            ctx = self->ctx;
        }
    } else {
        func_089B0D88(ctx, 0x200000, 0x200000);
        ctx = self->ctx;
    }
    func_089B47AC(ctx, idx, 0);
    if ((((E *)((char *)self->ents + off))->flags & 0x10000) != 0) {
        func_089A0C20(col);
    }
    func_089B0D88(self->ctx, 0x20, 1);
}
