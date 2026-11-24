/* Returns -1 when the field at +0x40C is null, otherwise the result of the lookup on it. */
extern int func_08A81188(int v);
int func_08A5BBE4(int p) {
    int v = *(int *)(p + 0x40C);
    if (v != 0) {
        return func_08A81188(v);
    } else {
        return -1;
    }
}
