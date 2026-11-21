/* 1 inside `near`, 0 beyond `far`, falling linearly in between, by the
 * distance between two points. */
#include "vfpu.h"

extern "C" {

float func_0896F87C(const VfpuVec4 *a, const VfpuVec4 *b, float near, float far)
{
    float d = Vfpu::Distance3(a, b);
    if (d > far) {
        return 0.0f;
    }
    if (d < near) {
        return 1.0f;
    }
    float range = far - near;
    return 1.0f - (d - near) / range;
}

}
