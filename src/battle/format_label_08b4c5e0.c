/* Formats a label from a name and number into a 0x80-byte buffer, creates a text object from it and passes it to func_08B4C264. */
extern const char D_08B4C698[];
extern void *D_089D6998;
extern int func_0887B500(int, int, int);
extern void func_089347E8(char *, const char *, int, int);
extern int func_0887D868(void *, int, char *, int, int, int, int, int);
extern void func_08B4C264(int, int, int, int);
void func_08B4C5E0(int a, int b, int c, int d, int e)
{
    char buf[0x80];
    int zero = 0;
    const char *fmt = D_08B4C698;
    int s = func_0887B500(d, e, 0x80);
    func_089347E8(buf, fmt, s, c);
    int t = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    func_08B4C264(a, b, t, zero);
}
