#include "main/agent21_types.h"

extern void *func_0891705C(int index);
extern void func_08912AF8(void *unit, int mode, int subMode);
extern void func_08916F78(int a, void (*done)(void));
extern void func_089168A4(Scene *scene, int frames);
extern void func_08914C18(Scene *scene);
extern void func_08914B64(Scene *scene, int state);
extern void func_08917B40(void);

void func_08914FD8(Scene *scene)
{
    void *unit = func_0891705C(-1);
    scene->field_060 = 0;
    scene->field_064 = 1;
    func_08912AF8(unit, 0, scene->field_064);
    func_08916F78(0, func_08917B40);
    func_089168A4(scene, 60);
    func_08914C18(scene);
    func_08914B64(scene, 9);
}
