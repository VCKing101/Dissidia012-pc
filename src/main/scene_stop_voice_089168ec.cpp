#include "main/agent21_types.h"

template <class T> struct Kept { T value; Kept(T v) : value(v) {} };

extern "C" {
extern void func_0889F340(int player, int frames);
extern void func_0889F2B8(int player);
extern void func_088A51E4(int id);
extern int D_089D6C30;

void func_089168EC(Scene *scene)
{
    if (scene->bgmHandle != 0) {
        Kept<Scene *> saved(scene);
        func_0889F340(D_089D6C30, 0);
        func_0889F2B8(D_089D6C30);
        func_088A51E4(saved.value->bgmId);
        Scene *s = saved.value;
        s->bgmId = -1;
        s->bgmHandle = 0;
    }
}
}
