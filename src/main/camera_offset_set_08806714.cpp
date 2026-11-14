#include "main/agent30_types.h"

/* Starts a camera move to an offset over the given duration. */
extern "C" {

void func_089A3868(Vec4 *dst, Vec4 *src);

void func_08806714(CameraBlend *c, float x, float y, float z, float duration)
{
    Vec4 v;
    Vec4 *pv = &v;
    c->offset_x = x;
    c->offset_y = y;
    c->offset_z = z;
    c->duration = duration;
    c->elapsed = 0.0f;
    c->distance = c->base_distance;
    v = c->pos;
    if (!c->started) {
        func_089A3868(&c->from, pv);
        func_089A3868(&c->to, pv);
        c->started = 1;
    }
    func_089A3868(&c->from, &c->to);
}

}
