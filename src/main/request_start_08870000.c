typedef struct Request08870000 {
    unsigned char unk_00[0x18];
    int e;                             /* +0x18 */
    int f;                             /* +0x1C */
} Request08870000;

extern Request08870000 D_09B26B18;
extern void func_0886FD08(void *self, int kind, int a, unsigned long long b, int c, int d);   /* request_init */
extern int func_08870154(void *self);   /* start the "MS Write" thread */

int func_08870000(void *self, int kind, unsigned long long b, int c, int d, int e, int f)
{
    if (d == 0) {
        return 0x80000016;
    }
    func_0886FD08(self, kind, 0, b, c, d);
    D_09B26B18.e = e;
    D_09B26B18.f = f;
    return func_08870154(self);
}
