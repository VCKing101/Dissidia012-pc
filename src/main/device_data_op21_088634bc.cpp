#include "main/agent37_types.h"

extern "C" {
extern char D_089C9118[];              /* "data/%s" */
int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

void func_088634BC(D37Device *self, const char *name)
{
    char path[0x100];

    func_0893476C(path, D_089C9118, name);
    self->op21(path);
}
}
