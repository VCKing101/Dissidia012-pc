/* Gets a string with func_08AC7990; if present, measures it, lays it out in the object's buffer at +0x1B0 and shows it with func_0887D868. */
typedef struct { char p0[0x1B0]; char buf[1]; } Buf;
typedef struct { char p0[0xC18]; Buf *p; } Obj;
extern int D_089D6998;
extern int func_08AC7990(Obj *self);
extern unsigned int func_0887E8C8(int s, int a, int b, int c);
extern void func_08AC7AB4(Obj *self, void *dst, int n, int s, unsigned int w);
extern int func_0887D868(int a, int b, void *c, int d, int e, int f, int g, int h);
int func_08AC77E8(Obj *self) {
    unsigned int w;
    int s = func_08AC7990(self);
    if (s == 0) {
        return 0;
    }
    w = func_0887E8C8(s, 0x400, 0, 2);
    func_08AC7AB4(self, self->p->buf, 0x400, s, w >> 1);
    return func_0887D868(D_089D6998, 0, self->p->buf, -1, 0, 0, 0, 0);
}
