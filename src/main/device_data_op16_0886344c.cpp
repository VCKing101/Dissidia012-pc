#include "main/agent37_types.h"

extern "C" {
extern char D_089C9118[];              /* "data/%s" */
int func_0893476C(char *dst, const char *format, ...);         /* sprintf */

/* Device virtual 16 with "data/<name>" as the path. */
void func_0886344C(D37Device *self, const char *name, int a, int b)
{
    char path[0x100];

    func_0893476C(path, D_089C9118, name);
    self->op16(path, a, b);
}
}
