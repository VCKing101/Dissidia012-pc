/* Menu page layout: loads the list and row entries; the no-flag branch also loads the table entry for the selection. */
typedef struct { char p[6]; unsigned short h6; } T8;
typedef struct {
    int f0; int f4; char a8[1]; char pB[3]; char aC[1];
    char p0[0xE8 - 0xD]; int fE8;
    char p1[0xBC0 - 0xEC]; char aBC0[1];
} B;
typedef struct { char p[0xC18]; B *b; } S;
extern T8 D_08ACA39E[];
extern char D_08ACA35E[], D_08ACA3CE[], D_08ACA38E[], D_08ACA3DE[], D_08ACA3FE[], D_08ACA3BE[], D_08ACA426[];
extern void func_088D3AE8(int, int, int, int);
extern void func_088D3F00(int, int, void *);
extern void func_088D44EC(int, void *, void *);
extern int func_088F14BC(void);
void func_08AC7F2C(S *self) {
    int s2;
    char *s1;
    s2 = self->b->f4;
    func_088D3F00(self->b->f0, s2, D_08ACA35E);
    s1 = (char *)self->b + 0xC;
    func_088D44EC(s2, s1, D_08ACA3CE);
    if (func_088F14BC() != 0) {
        func_088D3F00(self->b->f0, s2, D_08ACA38E);
        func_088D44EC(s2, s1, D_08ACA3DE);
    } else {
        func_088D3F00(self->b->f0, s2, D_08ACA39E);
        func_088D3AE8(s2, 0x52, D_08ACA39E[self->b->fE8].h6, 0x4E);
        func_088D44EC(s2, s1, D_08ACA3FE);
        func_088D44EC(s2, s1, (char *)self->b + 0xBC0);
        func_088D3F00(self->b->f0, s2, D_08ACA3BE);
        func_088D44EC(s2, s1, D_08ACA426);
    }
}
