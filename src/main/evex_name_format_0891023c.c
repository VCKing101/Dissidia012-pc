/* Formats "%s.evex" of the name at offset 0x18 into the 0x1F-byte buffer D_09AF0288 and returns it. */
extern char D_09AF0288[];
extern char D_089CE138[];
extern void func_089346D0(char *dst, int size, char *fmt, char *name);
char *func_0891023C(char *self)
{
    char *buf = D_09AF0288;
    func_089346D0(buf, 0x1F, D_089CE138, self + 0x18);
    return buf;
}
