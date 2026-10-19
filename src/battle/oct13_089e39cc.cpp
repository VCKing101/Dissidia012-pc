extern "C" {
#include "battle/runtime_helpers.h"
#include "battle/oct13_source_views.h"

extern void func_089E3FE4(void *);
extern void func_089E4594(void *);
extern void func_089E399C(void *);

void func_089E39CC(void *object)
{
    func_089E3FE4((unsigned char *)object + 0x70);
    func_089E4594((unsigned char *)object + 0x3C);
    func_089E4594((unsigned char *)object + 8);
    func_089E399C(object);
}
}
