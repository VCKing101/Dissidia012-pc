/* Runs a setup call, formats a name into a stack buffer, requests a handle and registers it. */
#include "menu/agent60_types.h"
extern void func_08B53ABC(S4C66C *self);
extern void func_089347E8(char *buf, const char *fmt, int arg);
extern void *func_0887B3EC(int a, int b, int c, int d, char **name, int e);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B53B80[];
extern const char D_08B53B70[];

void func_08B53A24(S4C66C *self, int a, int b, int c)
{
    char buf[128];
    char *name;
    func_08B53ABC(self);
    name = buf;
    func_089347E8(buf, D_08B53B80, c);
    self->f38 = (int)func_0887B3EC(a, b, 0, 0x36, &name, 1);
    func_088D428C(&self->f30, &self->f38, D_08B53B70, 1);
}
