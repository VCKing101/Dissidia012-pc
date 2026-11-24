/* Parses src against the string at 0x08B4BFEC, widens its eight halfwords at +0x10 into the int array **self and returns src + 0x20. */
struct Src { char pad[0x10]; short h[8]; };
extern char D_08B4BFEC[];
extern void func_08935970(struct Src *s, char *fmt);
struct Src *func_08B4B058(int **self, struct Src *src)
{
    int i;
    struct Src *s = src;
    int **p = self;
    func_08935970(s, D_08B4BFEC);
    for (i = 0; i < 8; i++) { (*p)[i] = s->h[i]; }
    return (struct Src *)((char *)s + 0x20);
}
