/* Applies the request to entry idx, or to every entry when idx is 0; true only if all accepted. */
typedef struct Ent { char pad[0x2C]; } Ent;
typedef struct Tbl { char pad[4]; Ent e[1]; } Tbl;
extern int func_08905910(void *, int, int);
extern int func_08905CAC(Tbl *, int);
int func_08905E68(Tbl *self, int a, int b, int idx)
{
    if (idx == 0) {
        int ok = 1;
        int i = 0;
        if (i < *(int *)((char *)self + 0x2C4)) {
            do {
                if (func_08905910((char *)self + i * 0x2C + 4, a, b) == 0) ok = 0;
                i++;
            } while (i < *(int *)((char *)self + 0x2C4));
        }
        return ok;
    } else {
        int r = func_08905CAC(self, idx);
        if (r >= 0) { return func_08905910((char *)self + r * 0x2C + 4, a, b); } else { return 0; }
    }
}
