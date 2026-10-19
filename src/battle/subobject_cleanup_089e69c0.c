#include "battle/runtime_helpers.h"

/* Called subobject dispatch wrappers retain their observed ABI and offsets. */
extern void func_089E9F04(void *);
extern void func_089E1E30(void *);
extern void func_089E6998(void *);

void func_089E69C0(unsigned char *object)
{
    func_089E9F04(object + 0x124);
    func_089E1E30(object + 8);
    func_089E6998(object);
}
