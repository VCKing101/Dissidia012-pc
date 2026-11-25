extern int func_0887B500(int, int, int);
extern void func_089347E8(void *, void *, int, int);
extern int func_08935E90(void *);
extern int func_0887D868(int, int, void *, int, int, int, int, int);
extern char D_08B4C698[];
extern int D_089D6998;

int func_08B4B040(int a1, int a2, int a3, int a4, int a5, int a6) {
    char buf[128];
    int n;
    n = 0;
    if (a1 != 0) {
        char *dst = buf + n * 2;
        char *fmt = D_08B4C698;
        int r = func_0887B500(a5, a6, 126);
        func_089347E8(dst, fmt, r, a1);
        n = func_08935E90(buf);
    }
    if (a2 != 0) {
        char *dst = buf + n * 2;
        char *fmt = D_08B4C698;
        int r = func_0887B500(a5, a6, 128);
        func_089347E8(dst, fmt, r, a2);
        n = func_08935E90(buf);
    }
    if (a3 != 0) {
        char *dst = buf + n * 2;
        char *fmt = D_08B4C698;
        int r = func_0887B500(a5, a6, 129);
        func_089347E8(dst, fmt, r, a3);
        n = func_08935E90(buf);
    }
    if (a4 != 0) {
        char *dst = buf + n * 2;
        char *fmt = D_08B4C698;
        int r = func_0887B500(a5, a6, 130);
        func_089347E8(dst, fmt, r, a4);
        n = func_08935E90(buf);
    }
    if (n != 0) {
        return func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    } else {
        return 0;
    }
}
