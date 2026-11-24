/* Initialises the object's sub-block via func_08864FF0, resets two fields of D_09B6CDA0, calls func_08AD1020 and returns self. */
typedef struct { char p0[0xE6]; unsigned char b; char p1; int w; } Glob;
extern Glob D_09B6CDA0;
extern void func_08864FF0(char *p, int a, int b, int c);
extern void func_08AD1020(void);
char *func_08AF2C30(char *self) {
    func_08864FF0(self + 0x100, 2, 0x11176, 0x43);
    D_09B6CDA0.b = 0;
    D_09B6CDA0.w = -1;
    func_08AD1020();
    return self;
}
