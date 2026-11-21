/* Reads one length-prefixed block from a stream cursor; returns -1 when the cursor is null. */
int func_08AB9FC0(int *n1, int *p1, int **cur, int s1)
{
    if (*cur == 0) {
        return -1;
    }
    *n1 = **cur;
    *cur = *cur + 1;
    *p1 = (int)*cur;
    *cur = (int *)((int)*cur + s1 * *n1);
    return 0;
}
