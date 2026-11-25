extern signed char D_089C8D8C[4];
extern signed char D_089C8D94[4];
extern int *func_0885CA98(int h, int a, int col, int i);
extern int func_0886646C(int a, int b);
#define COL1 (D_089C8D8C[0] | D_089C8D8C[1] << 8 | D_089C8D8C[2] << 16 | D_089C8D8C[3] << 24)
#define COL2 (D_089C8D94[0] | D_089C8D94[1] << 8 | D_089C8D94[2] << 16 | D_089C8D94[3] << 24)

void func_08857824(int *self, int h) {
    int *e;
    self[4] = self[4] + 1;
    if (*self != 0) {
        return;
    }
    {
        e = func_0885CA98(h, 3, COL1, 0);
        if (e != 0) {
            *self = func_0886646C(e[2], e[3]);
        }
        e = func_0885CA98(h, 3, COL1, 1);
        if (e != 0) {
            self[1] = func_0886646C(e[2], e[3]);
        }
        e = func_0885CA98(h, 3, COL1, 2);
        if (e != 0) {
            self[3] = func_0886646C(e[2], e[3]);
        }
        e = func_0885CA98(h, 3, COL2, 0);
        if (e != 0) {
            self[2] = func_0886646C(e[2], e[3]);
        }
    }
}
