#include "main/agent21_types.h"

extern int func_08873EB4(void);
extern void func_08873D34(float time, int id, void (*done)(void), int a, int b, int mode);
extern void func_08873DEC(int id, int mode);
extern void func_089168A4(Scene *scene, int frames);
extern void func_08914B74(Scene *scene);
extern void func_08916F5C(void);

void func_089149EC(Scene *scene)
{
    if (func_08873EB4() == 0) {
        func_08873D34(60.0f, 3, func_08916F5C, 0, 0, 2);
        func_08873DEC(3, 2);
        func_089168A4(scene, 60);
        func_08914B74(scene);
    }
}
