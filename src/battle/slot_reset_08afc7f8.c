/* Frees the two handles held in the object (if set) and clears the field at +0xC. */
extern void *D_089D6D78;
extern void *D_089D6C80;
extern void func_088A9B18(void);
extern void func_088A5604(void *table, int h);
extern void func_088A0DFC(void *table, int h);

void func_08AFC7F8(int *self)
{
    int none = -1;
    int *s = self;
    if (*s != 0) {
        func_088A9B18();
        func_088A5604(D_089D6D78, *s);
        *s = 0;
    }
    if (s[1] != none) {
        func_088A0DFC(D_089D6C80, s[1]);
        s[1] = none;
    }
    s[3] = 0;
}
