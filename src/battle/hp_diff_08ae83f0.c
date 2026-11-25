// Fills a two-fighter HP difference record (both directions) unless either fighter is in state 1.
struct Stats { char pad[0x14]; short hp; };
struct Core { char pad[0x52C]; struct Stats *stats; };
struct Fighter { char pad[0x10]; struct Core *core; struct Fighter *other; char pad2[0x18]; int st[1]; };
struct Rec { unsigned flags; int a, b, c, d, e; };
struct Out { char pad[0x10]; unsigned f[7]; };
extern int func_08AE9DE0(void *, int);

int func_08AE83F0(struct Out *out, struct Rec *rec, int unused, struct Fighter *f)
{
    struct Fighter *o = f->other;
    char *op = (char *)o + 0x30;
    int s1 = func_08AE9DE0((char *)f + 0x30, 2);
    int s2 = func_08AE9DE0(op, 2);
    int one;
    int two;
    if (s1 == (one = 1) || s2 == (two = one)) {
        return 0;
    }
    {
        int hp2 = f->other->core->stats->hp;
        int hp1 = f->core->stats->hp;
        rec->a = hp2;
        rec->b = hp2 - hp1;
        rec->flags |= 1;
        out->f[0] = hp1;
        out->f[1] = hp2;
        out->f[2] = hp2 - hp1;
        rec->c = hp1;
        rec->d = hp1 - hp2;
        rec->flags |= 2;
        unsigned f6 = out->f[6];
        out->f[3] = hp2;
        out->f[4] = hp1;
        out->f[5] = hp1 - hp2;
        out->f[6] = f6 | 4;
    }
    return 1;
}
