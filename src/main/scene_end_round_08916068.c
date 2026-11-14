#include "main/agent21_types.h"

extern void func_088AC8C8(int id, int on);
extern void func_08914B64(Scene *scene, int state);
extern void func_08864D98(float from, float to);
extern void func_08873D34(float time, int id, void (*done)(void), int a, int b, int mode);
extern void func_08873DEC(int id, int mode);
extern void func_089168A4(Scene *scene, int frames);
extern void func_08914C18(Scene *scene);
extern int D_089D712C;

void func_08916068(Scene *scene)
{
    func_088AC8C8(D_089D712C, 1);
    func_08914B64(scene, 17);
    func_08864D98(0.0f, 0.0f);
    func_08873D34(60.0f, 3, 0, 0, 0, 2);
    func_08873DEC(3, 2);
    func_089168A4(scene, 60);
    func_08914C18(scene);
}
