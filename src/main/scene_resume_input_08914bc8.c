#include "main/agent21_types.h"

extern void func_088A28E8(int a, int b);
extern void func_088965F4(int id, int on);
extern void func_088A3F2C(int on);
extern int D_089D6AE8;

void func_08914BC8(Scene *scene)
{
    if (scene->paused == 0) {
        return;
    }
    scene->paused = 0;
    func_088A28E8(0, 0);
    func_088965F4(D_089D6AE8, 0);
    func_088A3F2C(0);
}
