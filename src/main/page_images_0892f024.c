/* Loads the two images of a numbered page (by table entry, then by a formatted "page-sub" name) and registers them with the sprite sheet. */
typedef struct S { char p0[4]; char sheet[0x48]; int h0; int s0; int h1; int s1; char p1[0x60 - 0x5C]; int page; char p2[4]; int sub; char p3[0xCC - 0x6C]; int names[1]; } S;
extern int *D_089D6998;
extern char D_089CF0D0[];
extern char D_089CF058[];
extern void func_0892F10C(S *self);
extern int func_0887D868(void *p, int a, void *name, int b, int c, int d, int e, int f);
extern int func_0887E534(int h);
extern void func_089347E8(char *buf, char *fmt, int a, int b);
extern void func_088D428C(void *a, void *b, char *c, int d);
void func_0892F024(S *self)
{
    char buf[0x80];
    int page;
    int sub;
    func_0892F10C(self);
    page = self->page;
    sub = self->sub;
    self->h0 = func_0887D868(D_089D6998, 0, (void *)self->names[page], -1, 0, 0, 0, 0);
    self->s0 = func_0887E534(self->h0);
    func_089347E8(buf, D_089CF0D0, page + 1, sub + 1);
    self->h1 = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    self->s1 = func_0887E534(self->h1);
    func_088D428C(self->sheet, &self->h0, D_089CF058, 1);
}
