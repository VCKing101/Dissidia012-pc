typedef struct Request08870098 {
    unsigned char unk_00[0x14];
    int result;                        /* +0x14 */
    unsigned char unk_18[0xA0 - 0x18];
    unsigned char started;             /* +0xA0 */
} Request08870098;

extern void *D_089D5FF0;               /* the package reader */
extern unsigned long long D_089C9648;
extern int D_089D5FBC;                 /* the "MS Write" thread */
extern Request08870098 D_09B26B18;
extern int func_088716C8(void *package, int kind);
extern void func_0886FD08(void *self, int kind, int a, unsigned long long b, int c, int d);   /* request_init */
extern int func_08870154(void *self);   /* start the thread */
extern int func_089C6018(int thread, int unk);

int func_08870098(void *self, int kind, int a, int c, int d)
{
    func_088716C8(D_089D5FF0, kind);
    func_0886FD08(self, kind, a, D_089C9648, c, d);
    D_09B26B18.started = 1;
    if (func_08870154(self) > 0) {
        func_089C6018(D_089D5FBC, 0);
        return D_09B26B18.result;
    }
    return 0;
}
