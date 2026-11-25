/* Stores two values in a record; the offsets depend on whether its first byte is 'l'. */
typedef struct {
    unsigned char kind;
    char pad0[0x27];
    int a28; int a2c;
    char pad1[0x19C - 0x30];
    int b19c; int b1a0;
} Rec0894BE60;
void func_0894BE60(int a0, Rec0894BE60 *p, int x, int y) {
    Rec0894BE60 *q = p;
    if (q->kind == 'l') {
        p->a28 = x;
        p->a2c = y;
    } else {
        q->b19c = x;
        q->b1a0 = y;
    }
}
