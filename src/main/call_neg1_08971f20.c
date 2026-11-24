/* Returns func_08941D40(), mapping -1 to -1 (a lookup that may fail). */
extern int func_08941D40(void);
int func_08971F20(void) {
    int r = func_08941D40();
    if (r == -1) { return -1; }
    return r;
}
