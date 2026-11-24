// Tests bit 1 of the byte at offset 3 of the object func_088B9044 returns; 0 if none.
extern unsigned char *func_088B9044(void);
int func_088B912C(void)
{
    unsigned char *p = func_088B9044();
    if (p) return (p[3] & 2) != 0;
    return 0;
}
