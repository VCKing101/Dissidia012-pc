typedef struct Umd08863208 {
    unsigned char unk_000[0x158];
    int callback;                      /* +0x158 */
} Umd08863208;

extern char D_089C90F0[];              /* the callback's name */
extern char D_089C9110[];              /* the drive name */
extern int func_08863D4C(Umd08863208 *self);
extern int func_08863624(int a, int b, void *arg);   /* the UMD callback */
extern int func_089C5F80(const char *name, int (*fn)(int, int, void *), void *arg);  /* create callback */
extern int func_089C5F90(int callback);                                            /* delete callback */
extern int func_089C5BD0(int callback);       /* register the UMD callback */
extern int func_089C5BB8(void);               /* disc present */
extern int func_089C5BC8(int state);          /* wait for the drive state */
extern int func_089C5BE0(unsigned char mode, const char *drive);   /* activate */

int func_08863208(Umd08863208 *self)
{
    if (func_08863D4C(self) == 0) {
        return 0;
    }
    self->callback = func_089C5F80(D_089C90F0, func_08863624, self);
    if (self->callback < 0) {
        return 0;
    }
    if (func_089C5BD0(self->callback) < 0) {
        func_089C5F90(self->callback);
        return 0;
    }
    if (func_089C5BB8() == 0) {
        func_089C5BC8(2);
    }
    if (func_089C5BE0(1, D_089C9110) < 0) {
        return 0;
    }
    func_089C5BC8(0x20);
    return 1;
}
