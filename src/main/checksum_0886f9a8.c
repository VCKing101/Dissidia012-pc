int func_0886F9A8(int unused, signed char *data, int n)
{
    int i = 0;
    int sum = 0;
    for (; i < n; i++) {
        sum = (signed char)*((char *)i + (int)data) + (n - i) + sum;
    }
    return sum;
}
