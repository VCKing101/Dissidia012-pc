#include "main/agent21_types.h"

template <class T> struct Kept { T value; Kept(T v) : value(v) {} };

extern "C" {
extern void func_08914B74(Scene *scene);
extern void func_088D0B2C(int heap, int id);
extern void func_08913B58(Scene *scene, int mode);
extern void func_08914B64(Scene *scene, int state);
extern int D_089D7830;

void func_0891461C(Scene *scene)
{
    if (scene->timer < 16.0f) {
        return;
    }
    Kept<Scene *> saved(scene);
    func_08914B74(scene);
    saved.value->field_7E9 = 0;
    func_088D0B2C(D_089D7830, 0x24);
    func_08913B58(saved.value, 4);
    func_08914B64(saved.value, 11);
}
}
