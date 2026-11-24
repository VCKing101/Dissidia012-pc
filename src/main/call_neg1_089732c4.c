/* Returns func_089420F0(), mapping -1 to -1 (a lookup that may fail). */
extern int func_089420F0(void);
int func_089732C4(void) {
    int r = func_089420F0();
    if (r == -1) { return -1; }
    return r;
}
