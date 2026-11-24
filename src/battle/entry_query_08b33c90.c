// Returns whether func_088B7B88 reports non-zero for the selected 198-byte entry of the table at +0x30C.
#include "battle/agent50_types.h"
extern int func_088B7B88(void *, int);
typedef struct { char pad0[0x30C]; char *tbl; } Obj08B33C90;
int func_08B33C90(Obj08B33C90 *self, int b)
{
    char *t = self->tbl + 0x10;
    int idx = *(signed char *)(t + 0x763);
    return func_088B7B88(t + idx * 198, b) != 0;
}
