#include "main/agent21_types.h"

extern "C" {
void func_089153B8(Scene *scene)
{
    for (int i = 0; i < 4; i++) {
        if (scene->units[i] != 0) {
            scene->units[i]->release();
            scene->units[i] = 0;
        }
    }
}
}
