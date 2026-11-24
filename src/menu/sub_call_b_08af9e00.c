/* Returns 1 unless func_0892A1E0(&D_09B6CDA0+0xF0) is nonzero, in which case returns func_0892A21C of that block and the argument. */
typedef struct { char p0[0xF0]; char blk[1]; } Glob;
extern Glob D_09B6CDA0;
extern int func_0892A1E0(void *p);
extern int func_0892A21C(void *p, int a);
int func_08AF9E00(int a0, int a1) {
    int r = 1;
    void *p = D_09B6CDA0.blk;
    if (func_0892A1E0(p) != 0) {
        r = func_0892A21C(p, a1);
    }
    return r;
}
