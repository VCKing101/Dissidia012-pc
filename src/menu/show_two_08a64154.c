/* Frees the two text handles at +0x64 and +0x68, then shows the entry text of the current index and a formatted index+1 label, and registers them. */
typedef struct { char p0[0x1C]; char b1C[0x64 - 0x1C]; int w64; int w68; char p1[0x78 - 0x6C]; int w78; char p2[0xA4 - 0x7C]; int t[1]; } Obj;
extern int D_089D6998;
extern char D_08A682C0[];
extern char D_08A681D8[];
extern void func_0887BA5C(void);
extern int func_0887D868(int a, int b, void *c, int d, int e, int f, int g, int h);
extern void func_089347E8(void *dst, const char *fmt, int v);
extern void func_088D428C(void *a, void *b, void *c, int d);
void func_08A64154(Obj *arg) {
    Obj *self = arg;
    char buf[128];
    int idx;
    if (self->w64 != 0) {
        func_0887BA5C();
        self->w64 = 0;
    }
    if (self->w68 != 0) {
        func_0887BA5C();
        self->w68 = 0;
    }
    idx = self->w78;
    {
        int txt = self->t[idx];
        self->w64 = func_0887D868(D_089D6998, 0, (void *)txt, -1, 0, 0, 0, 0);
    }
    func_089347E8(buf, D_08A682C0, idx + 1);
    self->w68 = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    func_088D428C(self->b1C, &self->w64, D_08A681D8, 1);
}
