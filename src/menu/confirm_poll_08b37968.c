/* Polls the dialog result; 1 closes and moves to state 0x22, 2 runs the cancel handler. */
extern int func_08B46B84(void *d);
extern void func_08B46D9C(void *d);
extern void func_08B39B84(void *self);
extern void func_08B39A9C(void *self);
extern void func_088D4BF4(int a);

typedef struct S37968 { char pad[0xC4]; struct { char pad[0x10]; void *dlg; } *st; } S37968;

void func_08B37968(S37968 *self)
{
    void *d = self->st->dlg;
    switch (func_08B46B84(d)) {
    case 0:
        break;
    case 1:
        func_08B46D9C(d);
        func_08B39B84(self);
        func_088D4BF4(2);
        *(int *)self->st = 0x22;
        break;
    case 2:
        func_08B39A9C(self);
        func_088D4BF4(0);
        break;
    }
}
