#include "main/agent21_types.h"

extern "C" {
extern void func_08844DE0(int handle);
extern void func_08B453F8(int id);

void func_08915308(Scene *scene)
{
    for (int i = 0; i < 4; i++) {
        int open = scene->handles[i] > 0;
        if (open) {
            func_08844DE0(scene->handles[i]);
        }
    }
    func_08B453F8(scene->field_08C);
}
}
