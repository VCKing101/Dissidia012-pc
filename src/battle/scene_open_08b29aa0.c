#include "battle/agent27_types.h"

extern void func_08B29A30(SceneParts *scene);
extern void func_08B2C458(void *lights, int a, int c, int d);
extern void func_08B2BCA0(void *models, int a, int c, int d);
extern void func_08B2A0B8(void *effects, int a, int c);

int func_08B29AA0(SceneParts *scene, int a, int b, int c, int d)
{
    func_08B29A30(scene);
    if (a == 0 || b == 0 || c == 0 || d == 0) {
        return -1;
    }
    func_08B2C458(scene->lights, a, c, d);
    func_08B2BCA0(scene->models, a, c, d);
    func_08B2A0B8(scene->effects, a, c);
    scene->field_214 = a;
    scene->field_218 = b;
    scene->field_21C = c;
    scene->active = 1;
    return 0;
}
