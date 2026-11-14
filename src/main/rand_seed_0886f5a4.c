extern unsigned char D_089D5FD0;
void func_0886F5A4(int unused, unsigned char seed)
{
    D_089D5FD0 = seed;
}
unsigned char func_0886F5B4(void)
{
    D_089D5FD0 = D_089D5FD0 * 5 + 1;
    return D_089D5FD0;
}
