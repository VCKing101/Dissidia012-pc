#include "main/agent21_types.h"

template <class T> struct Kept { T value; Kept(T v) : value(v) {} };

extern "C" {
extern void func_08B49B04(int model);
extern void *func_089110B4(void);
extern void func_0885C538(void *pool, int model, int flags);

void func_08916324(Scene *scene)
{
    if (scene->field_570 == 0) {
        return;
    }
    Kept<Scene *> saved(scene);
    func_08B49B04(scene->field_570);
    void *pool = func_089110B4();
    func_0885C538(pool, saved.value->field_570, 0);
    Scene *s = saved.value;
    s->field_570 = 0;
    s->field_7E9 = 0;
}
}
