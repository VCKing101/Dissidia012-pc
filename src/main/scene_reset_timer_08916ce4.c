#include "main/agent21_types.h"

extern void func_08AC5BF8(void *timer);
extern float D_089CE2A4;

void func_08916CE4(Scene *scene)
{
    ScenePlayer *player = scene->player;
    if (player->timerDone == 0) {
        func_08AC5BF8(player->timer);
    }
    scene->field_7D0 = D_089CE2A4;
}
