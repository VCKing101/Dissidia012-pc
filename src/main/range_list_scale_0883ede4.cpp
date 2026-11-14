extern "C" {
typedef struct Range0883EDE4 { short lo; short hi; short percent; } Range0883EDE4;
typedef struct RangeNode0883EDE4 { int field_00; Range0883EDE4 range; short pad_0A; struct RangeNode0883EDE4 *next; } RangeNode0883EDE4;
struct RangeOps {
    static bool contains(Range0883EDE4 *r, int x) { return (x >= r->lo) & (x <= r->hi); }
};
int func_0883EDE4(int *value, RangeNode0883EDE4 *n, int x)
{
    int applied = 0;
    while (n != 0) {
        Range0883EDE4 *r = &n->range;
        if (RangeOps::contains(r, x)) {
            *value = *value * r->percent / 100;
            applied = 1;
        }
        n = n->next;
    }
    return applied;
}
}
