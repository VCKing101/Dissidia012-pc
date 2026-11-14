#include "main/agent30_types.h"

/* Starts a camera move of the given mode and duration from the current
   position. */
extern "C" {

void func_089A3868(Vec4 *dst, Vec4 *src);

void func_08806650(CameraBlend *c, int mode, float duration)
{
    Vec4 v;
    Vec4 *pv = &v;
    c->mode = mode;
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
