extern void func_08AECF30(void *a, void *b, int count, int flags);

void func_08AECEE4(void *a, void *b, int *count)
{
    if (*count > 0) {
        func_08AECF30(a, b, *count, 0);
    }
}
