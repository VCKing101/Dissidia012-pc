extern int func_08A82044(void *stream, int *out, unsigned int size);
extern void *func_08A81FC8(void *stream, unsigned int size);

/* Reads an element count, then allocates count * size bytes. */
int func_08A80220(int *count, void **data, void *stream, unsigned int size)
{
    int n;
    void *p;

    if (func_08A82044(stream, count, 4) != 0) {
        return -2;
    }
    n = *count;
    if (n < 0) {
        return -3;
    }
    if (n > 0) {
        p = func_08A81FC8(stream, size * n);
        *data = p;
        if (p == 0) {
            goto fail;
        }
    }
    return 0;
fail:
    return -4;
}
