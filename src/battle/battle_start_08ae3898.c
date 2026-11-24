/* Battle start: resets the sub-systems, picks the state from func_08AFFBA4, seeds the random value and primes the timers. */
typedef struct Table { char pad[0x800]; int field; } Table;
extern Table *D_089D6A10;
extern int D_08AE7CA0;
extern int D_09AF84D0;
extern int D_09B6C270;
extern int D_08AE7CAC;
extern int D_08AE7CB0;
extern float D_08AE7CA8;
extern int func_0889716C(int p);
extern void func_08ADFA08(void);
extern void func_08AE7F64(void);
extern void func_08AFE2E4(void);
extern int func_08AFFBA4(int v);

void func_08AE3898(void)
{
    int v = func_0889716C(D_089D6A10->field);
    func_08ADFA08();
    func_08AE7F64();
    func_08AFE2E4();
    D_08AE7CA0 = func_08AFFBA4(v);
    D_09B6C270 = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    D_08AE7CAC = 100;
    D_08AE7CB0 = 0;
    D_08AE7CA8 = 20.0f;
}
