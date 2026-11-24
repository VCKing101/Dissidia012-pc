// If func_08B1DBC8 succeeds, runs the handler for the phase word at +4 (0..6) and returns 0; else returns -1.
extern int func_08B1DBC8(void);
extern void func_08B1E298(void *);
extern void func_08B1F538(void *);
extern void func_08B1E4AC(void *);
extern void func_08B1F9AC(void *);
extern void func_08B1E584(void *);
extern void func_08B1E5D4(void *);
extern void func_08B1FB2C(void *);
typedef struct { int pad; int phase; } Obj08B1DB00;
int func_08B1DB00(Obj08B1DB00 *self)
{
    if (func_08B1DBC8() == 0) {
        return -1;
    }
    switch (self->phase) {
    case 0:
        func_08B1E298(self);
        break;
    case 1:
        func_08B1F538(self);
        break;
    case 2:
        func_08B1E4AC(self);
        break;
    case 3:
        func_08B1F9AC(self);
        break;
    case 4:
        func_08B1E584(self);
        break;
    case 5:
        func_08B1E5D4(self);
        break;
    case 6:
        func_08B1FB2C(self);
        break;
    }
    return 0;
}
