#include "battle/agent27_types.h"

extern int func_08B29B6C(SceneParts *scene);

int func_08B29B74(SceneParts *scene)
{
    if (func_08B29B6C(scene) == 0) {
        return 0;
    }
    return scene->field_004 != 0;
}
