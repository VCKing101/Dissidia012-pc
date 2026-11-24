/* Runs the setup call, creates up to two handles (one per enabled flag) and registers them. */
extern void func_08B52D54(void *self);
extern void *func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B52E78[];

typedef struct S52C74 { char pad[0x30]; int f30; int f34; int f38; void *h1; void *h2; } S52C74;

void func_08B52C74(S52C74 *self, unsigned char on1, unsigned char on2, int a, int b, int c, int d, int e)
{
    func_08B52D54(self);
    if (on1 != 0) {
        self->h1 = func_0887B3EC(a, b, 0, c, 0, 0);
    }
    if (on2 != 0) {
        self->h2 = func_0887B3EC(a, d, 0, e, 0, 0);
    }
    func_088D428C(&self->f30, &self->h1, D_08B52E78, 1);
}
