extern int func_08A21E28(void *stream, void *dst, int size);
extern void *func_08A21DAC(void *stream, int size);

int func_089EF1E8(int *count, void **out, void *stream, int elem_size)
{
    int n;
    void *data;
    if (func_08A21E28(stream, count, 4) != 0) {
        return -2;
    }
    n = *count;
    if (n < 0) {
        return -3;
    }
    if (n > 0) {
        data = func_08A21DAC(stream, elem_size * n);
        *out = data;
        if (data == 0) {
            return -4;
        }
    }
    return 0;
}
