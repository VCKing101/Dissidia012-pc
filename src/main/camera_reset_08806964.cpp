#include "main/agent30_types.h"

/* Starts a camera move back to the default mode with no offset. */
extern "C" {

void func_089A3868(Vec4 *dst, Vec4 *src);

void func_08806964(CameraBlend *c, float duration)
{
    Vec4 v;
    Vec4 *pv = &v;
    float zero = 0.0f;
    c->mode = 0;
    c->offset_z = zero;
    c->offset_y = zero;
    c->offset_x = zero;
    c->duration = duration;
    c->elapsed = zero;
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
