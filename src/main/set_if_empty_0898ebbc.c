/* Stores the second argument at offset 0x2C unless one is already set. */
void func_0898EBBC(int *p, int v) {
    int cur = p[11];
    if (cur) { return; }
    p[11] = v;
}
