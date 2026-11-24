/* Frees the old handle, formats a name into a stack buffer, requests a new handle and registers it. */
#include "menu/agent60_types.h"
extern void func_0887BA5C(void *h);
extern void func_089347E8(char *buf, const char *fmt, int arg);
extern void *func_0887B3EC(int a, int b, int c, int d, char **name, int e);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B4C7D0[];
extern const char D_08B4C7B0[];


void func_08B4C66C(S4C66C *self, int arg)
{
    char buf[128];
    char *name;
    if (self->h) {
        func_0887BA5C(self->h);
        self->h = 0;
    }
    name = buf;
    func_089347E8(buf, D_08B4C7D0, arg);
    self->h = func_0887B3EC(self->f40, self->f44, 0, 4, &name, 1);
    func_088D428C(&self->f30, &self->f38, D_08B4C7B0, 1);
}
