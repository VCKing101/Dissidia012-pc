/* Copies the current view matrix, runs the in-place transform on it and writes the result to dst. */
typedef struct M { int w[16]; } M;
extern char *D_089D6D80;
extern M *func_0885EA88(char *p);
extern void func_089A405C(M *a, M *b);
extern void func_08935524(void *dst, void *src, int n);
void func_088A81A8(int unused, void *dst)
{
    M m;
    m = *func_0885EA88(D_089D6D80 + 0x10);
    func_089A405C(&m, &m);
    func_08935524(dst, &m, 0x40);
}
