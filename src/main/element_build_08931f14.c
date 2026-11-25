/* Builds an element from a table row: runs both set-up steps with the row's fields and marks the element as built. */
typedef struct Row { char p0[0xC]; unsigned char c; unsigned char d; char p1; unsigned char sel; } Row;
extern unsigned short D_089CF3DC[];
extern Row *func_088BB88C(int id);
extern void func_08932090(int *self, int a, int b, int c, int d, int s);
extern void func_08932238(int *self, int a, int b, int c, int d, int e);
void func_08931F14(int *self, int a, int b, int c, int d, int e, int f, int id, int g)
{
    Row *r = func_088BB88C(id);
    if (r == 0) {
        return;
    }
    func_08932090(self, a, b, c, d, D_089CF3DC[r->sel]);
    func_08932238(self, e, f, r->c, r->d, g);
    *self = 1;
}
