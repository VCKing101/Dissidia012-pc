/* Finds the 0x2C-byte record whose 6-byte id equals the given one and passes it on with the two extra arguments; 0 when none (or no id). */
extern "C" int func_08935490(void *a, void *b, int n);
extern "C" int func_08905958(void *rec, int a, int b);
struct O08905F64 {
    char pad0[4]; char recs[0x2C0]; int count;
    static unsigned char same(void *a, void *b) { return func_08935490(a, b, 6) == 0; }
};
extern "C" int func_08905F64(O08905F64 *self, int a, int b, void *id)
{
    if (id != 0) {
        int i;
        char *p;
        for (i = 0, p = self->recs; i < self->count; i++, p += 0x2C) {
            char *q = p;
            if (O08905F64::same(q, id)) {
                return func_08905958(q, a, b);
            }
        }
    }
    return 0;
}
