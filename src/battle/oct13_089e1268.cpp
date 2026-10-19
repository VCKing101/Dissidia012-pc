extern "C" {
#include "battle/runtime_helpers.h"
#include "battle/oct13_source_views.h"

extern void func_0887BA5C(void *);
extern void func_089E1248(void *);

void func_089E1268(void *object)
{
    if (((BattleResource04View *)object)->field_04) {
        func_0887BA5C(((BattleResource04View *)object)->field_04);
        ((BattleResource04View *)object)->field_04 = 0;
    }
    func_089E1248(object);
}
}
