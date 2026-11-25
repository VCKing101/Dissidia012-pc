/* Looks up an entry with func_08900DA0; if found, calls func_08902994(D_089D712C + 0x3B7A4, entry byte 2, object byte 0, &object[0x10 + object[0x773] * 0xC6]). */
extern char *D_089D712C;
extern char *func_08900DA0(void);
extern void func_08902994(char *a, int b, int c, char *d);
void func_08900CE8(int unused, char *obj)
{
    char *e;
    { char *o = obj; e = func_08900DA0(); if (e != 0) {
        func_08902994(D_089D712C + 0x3B7A4, e[2], o[0], o + 0x10 + o[0x773] * 0xC6);
    } }
}
