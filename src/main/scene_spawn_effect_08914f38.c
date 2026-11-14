#include "main/agent21_types.h"

typedef struct Effect {
    char pad_000[0xDC];
    unsigned int flags;         /* 0xDC */
} Effect;

extern Effect *func_088A9DE8(int pool, int id, int a, int b, int c);
extern void func_088A7284(Effect *effect, float x, float y, float z);
extern float func_08837014(ScenePlayer *player);
extern void func_088A72E8(Effect *effect, float x, float y, float z);
extern void func_08914B64(Scene *scene, int state);
extern int *D_089D8378;

void func_08914F38(Scene *scene)
{
    Effect *effect = func_088A9DE8(*D_089D8378, 9, 0, 0, 0);
    float scale;
    func_088A7284(effect, scene->player->pos[0], scene->player->pos[1], scene->player->pos[2]);
    scale = func_08837014(scene->player);
    func_088A72E8(effect, scale, scale, scale);
    effect->flags |= 0x100;
    scene->field_048 = 3;
    func_08914B64(scene, 8);
}
