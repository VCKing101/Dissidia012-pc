/* Finds how many days ahead the stored time stamp matches, adding a week when the flag is set. */
typedef struct Rec { unsigned short a; unsigned char b; unsigned char c; } Rec;
typedef struct Big { char pad[0x1DE84]; Rec rec; } Big;
extern Big *D_089D712C;
extern void func_088EFDC8(char *out, unsigned short a, unsigned short b, unsigned short c);
extern void func_089C5D78(char *in, char *out);
extern void func_089C5D90(char *a, char *b, int n);
extern void func_089C5D80(char *a, char *b);
extern int func_089C5D70(int a, int b, int c);
extern int func_088F0C80(void);
extern int func_088F1094(void);
int func_088F1360(void)
{
    unsigned short now[8];
    char t[8];
    int r;
    int i;
    Rec *rec = &D_089D712C->rec;
    func_088EFDC8((char *)now, rec->a, rec->b, rec->c);
    r = 0;
    i = 0;
    do {
        int x = func_088F0C80();
        int y = func_089C5D70(now[0], now[1], now[2]);
        if (x == y) {
            r = i;
            break;
        }
        func_089C5D78((char *)now, t);
        func_089C5D90(t, t, 1);
        func_089C5D80((char *)now, t);
        i++;
    } while (i < 7);
    if (func_088F1094() != 0) {
        r = r + 7;
    }
    return r;
}
