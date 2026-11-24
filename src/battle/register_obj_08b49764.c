// Runs func_08B497E4, builds an object with func_0887B3EC from a1/a2/a3 (stored at +0x20) and registers it with func_088D428C.
extern void func_08B497E4(void *, int, int, int);
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_088D428C(void *, void *, void (*)(void), int);
extern void func_08B49870(void);
typedef struct { char pad0[4]; char a[0x1C]; int obj; } Obj08B49764;
void func_08B49764(Obj08B49764 *self0, int a10, int a20, int a30)
{
    int a3 = a30;
    int a2 = a20;
    Obj08B49764 *self = self0;
    int a1 = a10;
    func_08B497E4(self, a1, a2, a3);
    self->obj = func_0887B3EC(a1, a2, 0, a3, 0, 0);
    func_088D428C(self->a, &self->obj, func_08B49870, 1);
}
