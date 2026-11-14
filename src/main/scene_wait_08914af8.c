#include "main/agent21_types.h"

extern int func_088AC9A8(int id);
extern void func_089136C0(Scene *scene);
extern int D_089D712C;

void func_08914AF8(Scene *scene)
{
    if (scene->waiting != 0) {
        if (func_088AC9A8(D_089D712C) == 0 || scene->timer < 60.0f) {
            return;
        }
        scene->waiting = 0;
    }
    func_089136C0(scene);
}
