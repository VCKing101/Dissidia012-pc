#include "main/agent21_types.h"

extern void func_08873D34(float time, int id, void (*done)(void), int a, int b, int mode);
extern void func_08873DEC(int id, int mode);
extern void func_08914B64(Scene *scene, int state);

void func_08916A84(Scene *scene, int result)
{
    func_08873D34(16.0f, 3, 0, 0, 0, 2);
    func_08873DEC(3, 2);
    func_08914B64(scene, 12);
    scene->field_7CC = result;
}
