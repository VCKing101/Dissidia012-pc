extern signed char D_089CEA88[4];
extern int func_08926048(void *a, int b);
extern int func_0885C9E4(int h, int a, int col);
extern int *func_0885CA98(int h, int a, int col, int i);
extern void func_0892624C(void *a, void *b, int c, void *col, int p, int q);
#define COL (D_089CEA88[0] | D_089CEA88[1] << 8 | D_089CEA88[2] << 16 | D_089CEA88[3] << 24)
void func_0892631C(void *a, void *b, short id) {
    int h = func_08926048(a, id);
    int n = func_0885C9E4(h, 1, COL);
    int i;
    for (i = 0; i < n; i++) {
        {
            int *e = func_0885CA98(h, 1, COL, i);
            if (e == 0) {
                return;
            }
            func_0892624C(a, b, 1, D_089CEA88, e[2], e[3]);
        }
    }
}
