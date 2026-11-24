// Finds the index of the 0x18-byte entry in table D_089D7838 whose name matches; 0 when not found.
extern char D_089D7838[];
extern int func_0893585C(const char *, const char *);
int func_088D51D0(const char *name)
{
    int found = 0;
    int i = 0;
    char *p = D_089D7838;
    for (;;) {
        if (*p == 0) break;
        if (func_0893585C(p, name) == 0) {
            found = i;
            break;
        }
        p += 0x18;
        i++;
    }
    return found;
}
