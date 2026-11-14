#include "main/agent21_types.h"

extern "C" {
void func_08915370(Scene *scene)
{
    if (scene->player != 0) {
        scene->player->release();
        scene->player = 0;
    }
}
}
