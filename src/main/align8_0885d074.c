unsigned int func_0885D074(int unused, unsigned int n)
{
    unsigned int rem = n & 7;
    if (rem != 0) {
        return n - rem + 8;
    } else {
        return n;
    }
}
