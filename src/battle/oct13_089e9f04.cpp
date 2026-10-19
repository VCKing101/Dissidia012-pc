extern "C" {
#include "battle/runtime_helpers.h"
#include "battle/oct13_source_views.h"

extern void func_089EA74C(void *);
extern void func_088DA060(void *);

void func_089E9F04(void *object)
{
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 0x1C;
        do {
            func_089EA74C(entry);
            ++i;
            entry += 0x84C;
        } while (i < 2);
    }
    if (((BattleResource18View *)object)->field_18) {
        func_088DA060(((BattleResource18View *)object)->field_18);
        ((BattleResource18View *)object)->field_18 = 0;
    }
    func_089E9EF4((unsigned char *)object);
}
}
