/* Drops a reference; the last one releases all four objects. */
typedef struct Set088579E4 {
    void *objs[4];
    int refs;
} Set088579E4;

void func_0899BB1C(void *obj);

void func_088579E4(Set088579E4 *s)
{
    int i;
    int n = s->refs - 1;
    s->refs = n;
    if (n <= 0) {
        for (i = 0; i < 4; i++) {
            func_0899BB1C(s->objs[i]);
            s->objs[i] = 0;
        }
    }
}
