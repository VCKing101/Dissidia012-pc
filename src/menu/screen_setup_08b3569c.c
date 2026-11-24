/* Resets the display state, loads the title resources, creates the list object once and shows the first page. */
extern void func_08933C00(void *a);
extern void func_08866AA8(int a, int b, int c);
extern void func_088A0D40(void);
extern void func_08B42AB0(int a);
extern int func_08B42B40(void);
extern void *func_0885C6CC(int a, int b, int c, int d, int e, int f);
extern void func_08B44F10(void *p);
extern void func_08B44F40(void);
extern void func_0891F688(void *a, int b, int c);
extern void *D_089D85E8;
extern void *D_089D8520;
extern int D_08B42C28;

typedef struct S3569C { char pad[0xAC]; int fAC; int fB0; char padB4[4]; void *list; } S3569C;

void func_08B3569C(S3569C *self)
{
    void *p;
    void *r;
    int id;
    int one;
    func_08933C00(D_089D85E8);
    func_08866AA8(0, 0, 0);
    func_088A0D40();
    func_08B42AB0(D_08B42C28);
    id = func_08B42B40();
    one = 1;
    p = self->list = func_0885C6CC(id, 4, 0, 0, one, 0);
    r = 0;
    if (p != 0) {
        func_08B44F10(p);
        r = p;
    }
    self->list = r;
    func_08B44F40();
    id = func_08B42B40();
    func_0891F688(D_089D8520, 0, id);
    self->fAC = 0;
    self->fB0 = 0;
}
