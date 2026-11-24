/* Stores the selected index into *out_idx and copies the selected record into dst from one of two tables, or clears it. */
typedef struct Rec { int w[0x55]; } Rec;
typedef struct Ent { char pad0[0x54]; int f54; char pad1[0x194 - 0x58]; int idx; } Ent;
extern char *D_089D712C;
extern char *D_089D76F0;
extern unsigned func_08A5738C(Ent *, int);
extern int func_08A573A0(Ent *, int);
extern void func_08929AFC(void *, void *, int, int);
extern void func_0892A13C(void *);
void func_08A56E30(Ent *self, short *out_idx, Rec *dst) {
    unsigned kind;
    int n;
    *out_idx = self->idx;
    kind = func_08A5738C(self, self->idx);
    n = func_08A573A0(self, self->idx);
    if (kind == 1) {
        if (n >= 0 && n < 5) {
            *dst = *(Rec *)(D_089D712C + n * 0x154 + 0x3E0B0);
        }
    } else if (kind == 2) {
        char *p = D_089D76F0 + n * 0x13C;
        if (p) func_08929AFC(dst, p, self->f54, 0xC);
    } else {
        func_0892A13C(dst);
    }
}
