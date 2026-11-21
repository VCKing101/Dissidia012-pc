/* String library (built with -O3): does <base>/<dir>/<name> exist? 1 yes, 0 no (0x80010002 from the
 * stat call), another error as returned, 0x800001FE when the path would not fit in 0x400 bytes. */
unsigned int func_089AB27C(const char *s, unsigned int max);
void func_089AB210(char *dst, const char *src, unsigned int size);
void func_089AB2B4(char *dst, const char *src, unsigned int size);
int func_089C5E68(const char *path, void *stat); /* I: sceIoGetstat */
extern char D_089DAA68[];          /* base directory */
extern unsigned int D_089DAA54;    /* I: bytes reserved after the path */
extern const char D_089D0A04[];    /* separator string */

int func_089AB038(const char *dir, const char *name)
{
    char stat[0x60];
    char path[0x400];
    int r;

    if (func_089AB27C(dir, 0x400) + func_089AB27C(name, 0x400) + func_089AB27C(D_089DAA68, 0x400) + D_089DAA54 >= 0x400)
        return 0x800001FE;
    func_089AB210(path, D_089DAA68, 0x400);
    func_089AB2B4(path, D_089D0A04, 0x400);
    func_089AB2B4(path, dir, 0x400);
    func_089AB2B4(path, D_089D0A04, 0x400);
    func_089AB2B4(path, name, 0x400);
    r = func_089C5E68(path, stat);
    if (r == 0)
        r = 1;
    else if (r == 0x80010002)
        r = 0;
    return r;
}
