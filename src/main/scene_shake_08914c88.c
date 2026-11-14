#include "main/agent21_types.h"

extern void func_088EB0FC(float scale, float speed, void *lock, int a, int b, int c, int d);
extern void func_08914C18(Scene *scene);

void func_08914C88(Scene *scene)
{
    func_088EB0FC(0.5f, 0.02f, scene->lock, 90, 0, 150, 60);
    func_08914C18(scene);
}
