/* Copies 0xC0 bytes from a1 into D_09B6CDA0+8; when func_08AF9F1C(D_08B01CF8) is nonzero and word +0xC matches +0xC0, resets +0xC0 and sets +0xC4. */
typedef struct { int w[48]; } Big;
typedef struct { char p0[8]; Big b; } Glob;
extern Glob D_09B6CDA0;
extern int D_08B01CF8;
extern int func_08AF9F1C(int a);
void func_08AF34B0(int a0, Big *src) {
    D_09B6CDA0.b = *src;
    if (func_08AF9F1C(D_08B01CF8) != 0) {
        if (D_09B6CDA0.b.w[1] == D_09B6CDA0.b.w[46]) {
            D_09B6CDA0.b.w[46] = 0;
            D_09B6CDA0.b.w[47] = 1;
        }
    }
}
