/* Returns func_08941FC0(), mapping -1 to -1 (a lookup that may fail). */
extern int func_08941FC0(void);
int func_0897328C(void) {
    int r = func_08941FC0();
    if (r == -1) { return -1; }
    return r;
}
