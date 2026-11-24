/* Convert the stored and current time stamps and return the elapsed amount, at least 1. */
typedef struct Rec { unsigned short a; unsigned char b; unsigned char c; } Rec;
typedef struct Big { char pad[0x1DE80]; Rec rec; } Big;
extern Big *D_089D712C;
extern void func_088EFDC8(char *out, unsigned short a, unsigned short b, unsigned short c);
extern void func_089C5D78(char *in, char *out);
extern void func_089C5D98(unsigned short *now);
extern int func_088EFCD8(char *a, char *b);
int func_088F1D7C(void)
{
    unsigned short now[8];
    char t0[16];
    char t1[8];
    char t2[8];
    int r;
    Rec *rec = &D_089D712C->rec;
    func_088EFDC8(t0, rec->a, rec->b, rec->c);
    func_089C5D78(t0, t1);
    func_089C5D98(now);
    func_088EFDC8(t0, now[0], now[1], now[2]);
    func_089C5D78(t0, t2);
    r = func_088EFCD8(t2, t1);
    if (r > 0) {
    } else {
        r = 1;
    }
    return r;
}
