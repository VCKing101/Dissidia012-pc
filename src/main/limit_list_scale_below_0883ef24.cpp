extern "C" {
typedef struct Limit0883EF24 { short limit; short percent; } Limit0883EF24;
typedef struct LimitNode0883EF24 { int field_00; Limit0883EF24 limit; int pad_08; struct LimitNode0883EF24 *next; } LimitNode0883EF24;
struct LimitOps {
    static bool reached(Limit0883EF24 *l, float x) { return !(l->limit < x); }
};
int func_0883EF24(int *value, LimitNode0883EF24 *n, float x)
{
    int applied = 0;
    while (n != 0) {
        Limit0883EF24 *l = &n->limit;
        if (LimitOps::reached(l, x)) {
            *value = *value * l->percent / 100;
            applied = 1;
        }
        n = n->next;
    }
    return applied;
}
}
