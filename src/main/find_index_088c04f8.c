// Searches a 0x1F-byte table at D_089CBA68 for a key and returns its index, or -1.
extern char *func_088C054C(char *, char *, int *);
extern char D_089CBA68[];
int func_088C04F8(int key)
{
    char *begin = D_089CBA68;
    char *end = D_089CBA68 + 0x1F;
    char *r = func_088C054C(begin, end, &key);
    int idx = -1;
    if (r != end) idx = r - begin;
    return idx;
}
