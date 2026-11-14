extern "C" {
typedef struct Limit0883EF9C { short limit; short percent; } Limit0883EF9C;
typedef struct LimitNode0883EF9C { int field_00; Limit0883EF9C limit; int pad_08; struct LimitNode0883EF9C *next; } LimitNode0883EF9C;
struct LimitOps {
    static bool passed(Limit0883EF9C *l, float x) { return l->limit <= x; }
};
int func_0883EF9C(int *value, LimitNode0883EF9C *n, float x)
{
    int applied = 0;
    while (n != 0) {
        Limit0883EF9C *l = &n->limit;
        if (LimitOps::passed(l, x)) {
            *value = *value * l->percent / 100;
            applied = 1;
        }
        n = n->next;
    }
    return applied;
}
}
