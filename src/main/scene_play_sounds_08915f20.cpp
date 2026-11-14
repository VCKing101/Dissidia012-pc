#include "main/agent21_types.h"

extern "C" {
extern void func_088E04F0(int bank, int id);

void func_08915F20(Scene *scene, short *ids, int count)
{
    for (int i = 0; i < count; i++) {
        func_088E04F0(scene->sounds, ids[i]);
    }
}
}
