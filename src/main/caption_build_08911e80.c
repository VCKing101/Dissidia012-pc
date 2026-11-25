/* Builds the "%s Lv%d" caption, creates its handle (stored at 0x14) and four copies of it at 0x18.., then registers it. */
extern int D_089D6998;
extern char D_089CE250[];
extern char D_089CE200[];
void func_08911F64(int *self);
int func_0887B500(int a, int b, int c);
void func_089347E8(char *buf, char *fmt, int name, int lv);
int func_0887D868(int a, int b, char *c, int d, int e, int f, int g, int h);
int func_0887E534(int h);
void func_088D428C(int *self, int *h, char *s, int n);
void func_08911E80(int *self, int a, int b, int c, int lv)
{
    char buf[0x80];
    int i;
    int *p;
    int nm;
    func_08911F64(self);
    nm = func_0887B500(a, b, c);
    func_089347E8(buf, D_089CE250, nm, lv);
    self[5] = func_0887D868(D_089D6998, 0, buf, -1, 0, 0, 0, 0);
    for (i = 1, p = self + 1; i < 5; i++, p++) {
        p[5] = func_0887E534(self[5]);
    }
    func_088D428C(self, &self[5], D_089CE200, 1);
}
