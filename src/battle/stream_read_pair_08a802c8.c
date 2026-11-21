/* Reads two length-prefixed blocks from a stream cursor; returns -1 when the cursor is null. */
int func_08A802C8(int *n1, int *p1, int *n2, int *p2, int **cur, int s1, int s2)
{
    if (*cur == 0) {
        return -1;
    }
    *n1 = **cur;
    *cur = *cur + 1;
    *p1 = (int)*cur;
    *cur = (int *)((int)*cur + s1 * *n1);
    *n2 = **cur;
    *cur = *cur + 1;
    *p2 = (int)*cur;
    *cur = (int *)((int)*cur + s2 * *n2);
    return 0;
}
