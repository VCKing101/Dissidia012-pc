/* Returns func_08941DC0(), mapping -1 to -1 (a lookup that may fail). */
extern int func_08941DC0(void);
int func_08973254(void) {
    int r = func_08941DC0();
    if (r == -1) { return -1; }
    return r;
}
