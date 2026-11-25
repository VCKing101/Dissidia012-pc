/* Collects the four neighbour results (two lookups, both directions) that differ from key; returns the count. */
extern int func_08B4A6A8(int, int, int, int);
extern int func_08B4A7D8(int, int, int, int);
int func_08B4ABD8(int self, unsigned key, int *out) {
    int n = 0;
    int r;
    r = func_08B4A6A8(self, key, 1, 0);
    if ((unsigned)r != key) {
        out[0] = r;
        n = 1;
    }
    r = func_08B4A6A8(self, key, -1, 0);
    if ((unsigned)r != key) {
        out[n] = r;
        n++;
    }
    r = func_08B4A7D8(self, key, 1, 0);
    if ((unsigned)r != key) {
        out[n] = r;
        n++;
    }
    r = func_08B4A7D8(self, key, -1, 0);
    if ((unsigned)r != key) {
        out[n] = r;
        n++;
    }
    return n;
}
