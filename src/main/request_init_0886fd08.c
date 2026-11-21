typedef struct Request0886FD08 {
    int kind;                          /* +0x00 */
    int c;                             /* +0x04 */
    unsigned long long b;              /* +0x08 */
    int d;                             /* +0x10 */
    int a;                             /* +0x14 */
    unsigned char unk_18[0xA8 - 0x18];
} Request0886FD08;

extern Request0886FD08 D_09B26B18;
extern void *func_08935828(void *dst, int c, unsigned int n);   /* memset */

/* request_init: clear the 0xA8-byte request at 0x09B26B18 and fill it in. */
void func_0886FD08(void *self, int kind, int a, unsigned long long b, int c, int d)
{
    func_08935828(&D_09B26B18, 0, sizeof(D_09B26B18));
    D_09B26B18.kind = kind;
    D_09B26B18.a = a;
    D_09B26B18.b = b;
    D_09B26B18.c = c;
    D_09B26B18.d = d;
}
