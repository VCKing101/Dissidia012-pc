typedef struct Flag0887263C {
    unsigned long long key;
    unsigned long long value;
} Flag0887263C;

extern unsigned long long D_089C9A68;  /* the table end key (and the empty result) */

unsigned long long func_0887263C(void *self, unsigned long long mask, const Flag0887263C *e)
{
    unsigned long long result = D_089C9A68;

    for (; e->key != D_089C9A68; e++) {
        if ((mask & e->key) == e->key) {
            result |= e->value;
        }
    }
    return result;
}
