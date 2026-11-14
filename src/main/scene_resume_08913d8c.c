#include "main/agent21_types.h"

extern void func_08B43A3C(int a, ScenePlayer *player);
extern int func_08873EB4(void);
extern void func_08916854(Scene *scene);
extern void func_08914F38(Scene *scene);
extern void func_08914B64(Scene *scene, int state);
extern unsigned char *D_089D69E0;

void func_08913D8C(Scene *scene)
{
    unsigned char zero;
    func_08B43A3C(scene->field_088, scene->player);
    if (func_08873EB4() != 0) {
        return;
    }
    zero = 0;
    *D_089D69E0 = zero;
    func_08916854(scene);
    if (scene->field_7E4 != 0) {
        func_08914F38(scene);
        scene->field_7E4 = 0;
    } else {
        func_08914B64(scene, 2);
    }
}
