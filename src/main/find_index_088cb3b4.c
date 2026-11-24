// Searches a 0x1F-byte table at D_089CBC98 for a key and returns its index, or -1.
extern char *func_088CB2C0(char *, char *, int *);
extern char D_089CBC98[];
int func_088CB3B4(int key)
{
    char *begin = D_089CBC98;
    char *end = D_089CBC98 + 0x1F;
    char *r = func_088CB2C0(begin, end, &key);
    int idx = -1;
    if (r != end) idx = r - begin;
    return idx;
}
