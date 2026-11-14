#include "main/agent21_types.h"

extern void func_08864D98(float from, float to);
extern void func_08864DB8(void);
extern int func_08B4A054(int id);
extern void func_08914B64(Scene *scene, int state);

void func_0891453C(Scene *scene)
{
    SceneActor *actor = scene->player->actor;
    unsigned char zero = 0;
    actor->hidden = zero;
    func_08864D98(0.0f, 1.0f);
    if (func_08B4A054(scene->field_570) != 0) {
        actor->hidden = 1;
        func_08864DB8();
        func_08914B64(scene, 2);
    }
}
