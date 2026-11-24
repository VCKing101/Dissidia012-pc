/* Refresh the current stamp and report whether it differs from the stored one, updating the valid flag. */
typedef struct Stamp { unsigned short a, b, c; } Stamp;
typedef struct Rec { unsigned short a; unsigned char b; unsigned char c; } Rec;
typedef struct Big { char pad[0x1DE84]; Rec rec; } Big;
extern Stamp D_09B698C8;
extern char D_089D7F1B;
extern Big *D_089D712C;
extern void func_088EFDAC(Stamp *s);
int func_088F07F4(void)
{
    int r = 0;
    Rec *rec;
    func_088EFDAC(&D_09B698C8);
    rec = &D_089D712C->rec;
    if (rec->a != D_09B698C8.a || rec->b != D_09B698C8.b
        || rec->c != D_09B698C8.c) {
        D_089D7F1B = 1;
        if (rec->a == 0 || rec->b == 0) {
            D_089D7F1B = 0;
        }
        r = 1;
    }
    return r;
}
