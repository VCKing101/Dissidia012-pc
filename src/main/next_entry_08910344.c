/* Return the entry after the given one (0x78-byte entries), or 0 when past the end. */
int func_08910344(int self, int cur)
{
    int list = *(int *)(self + 4);
    int zero = 0;
    int r = zero;
    if (cur == 0) {
        if (zero < *(int *)(list + 4)) {
            return list + 8;
        }
        return r;
    } else {
        int first = list + 8;
        int n = (cur - first) / 0x78 + 1;
        if (n < *(int *)(list + 4)) {
            return list + n * 0x78 + 8;
        }
    }
    return r;
}
