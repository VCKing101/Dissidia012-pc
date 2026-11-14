#include "main/agent21_types.h"

extern void func_0889F2D4(int player, int id);
extern void func_08895F60(int id);
extern int D_089D6C30;
extern int D_089D6AE8;

void func_08916854(Scene *scene)
{
    unsigned char bgm = scene->info->bgm;
    if (bgm == 0xFF) {
        return;
    }
    func_0889F2D4(D_089D6C30, bgm);
    func_08895F60(D_089D6AE8);
}
