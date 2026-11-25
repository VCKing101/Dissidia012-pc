/* Collects ids in three ranges that the check rejects, then picks one at random and applies it. */
extern void *func_08899300(void);
extern int func_0889877C(void *p, int id);
extern void *func_0881BAA8(void);
extern void func_0881BC9C(void *p, int id, int v);

extern void func_08B4F794(int *self)
{
    int ids[100];
    int n;
    int *p;
    int i;
    n = 0;
    i = 0x2C;
    p = ids;
    for (; i < 0x33; i++) {
        if (func_0889877C(func_08899300(), i) == 0) {
            *p = i;
            n++;
            p++;
        }
    }
    for (i = 0; i < 0x19; i++) {
        if (func_0889877C(func_08899300(), i) == 0) {
            *p = i;
            n++;
            p++;
        }
    }
    for (i = 0x4C; i < 0x51; i++) {
        if (func_0889877C(func_08899300(), i) == 0) {
            *p = i;
            n++;
            p++;
        }
    }
    if (n == 0) {
        return;
    }
    {
        int idx;
        *self = *self * 0x343FD + 0x269EC3;
        idx = ((*self >> 16) & 0x7FFF) % n;
        func_0881BC9C(func_0881BAA8(), ids[idx], 0x3F2);
    }
}
