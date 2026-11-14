extern "C" {
/* Returns the nth entry whose name matches the key, or 0. */
typedef struct Entry08869B60 {
    void *name;
} Entry08869B60;

typedef struct Info08869B60 {
    int field_00;
    int field_04;
    int count;
} Info08869B60;

typedef struct Set08869B60 {
    Info08869B60 *info;
} Set08869B60;

int func_089359E8(void *key);
int func_08935A0C(void *key, void *name, int hash);
Entry08869B60 *func_08869A7C(Set08869B60 *set, int index);

Entry08869B60 *func_08869B60(Set08869B60 *set, void *key, int nth)
{
    int count = set->info->count;
    int hash = func_089359E8(key);
    int n = 0;
    for (int i = 0; i < count; i++) {
        Entry08869B60 *e = func_08869A7C(set, i);
        if (e->name != 0 && func_08935A0C(key, e->name, hash) == 0) {
            if (n == nth) {
                return e;
            }
            n++;
        }
    }
    return 0;
}
}
