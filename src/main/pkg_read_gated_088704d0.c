extern unsigned char D_089D5FD1;
extern unsigned char D_089D5FD2;
extern unsigned char D_089D5FD3;
extern unsigned char D_089D5FD5;
extern void *D_089D5FF0;               /* the package reader */
extern int func_088F47E0(int id);
extern void *func_088713E4(void *package, const char *path);   /* pkg_index_find */
extern int func_0886FB98(void *entry);
extern int func_088716C8(void *package, void *entry);

int func_088704D0(void *self, const char *path)
{
    void *entry;

    if (D_089D5FD1 == 0 || D_089D5FD2 != 0 || D_089D5FD5 != 0) {
        return 0;
    }
    if (D_089D5FD3 != 0 && !(unsigned char)(func_088F47E0(0x14) != 0)) {
        return 0;
    }
    entry = func_088713E4(D_089D5FF0, path);
    if (func_0886FB98(entry) != 0) {
        return func_088716C8(D_089D5FF0, entry);
    }
    return 0;
}
