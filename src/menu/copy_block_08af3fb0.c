/* Copies a 0x160C-byte block from self->p140+0x13E8 into D_089D712C+0xF6E0. */
typedef struct { int w[1411]; } Big;
typedef struct { char p0[0x140]; char *p140; } Obj;
extern char *D_089D712C;
void func_08AF3FB0(Obj *self) {
    Big *dst = (Big *)(D_089D712C + 0xF6E0);
    Big *src = (Big *)(self->p140 + 0x13E8);
    *dst = *src;
}
