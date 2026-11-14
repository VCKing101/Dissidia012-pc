/* Stores the value, or 0 when it is null. */
void func_088BBD44(int *out, int value)
{
    int zero = 0;
    *out = value != 0 ? value : zero;
}
