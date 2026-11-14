#include "main/agent30_types.h"

/* Sphere attack task constructor: builds the base task, then sets the
   attack's definition, centre and radius. */
struct SphereTask0880B768 {
    char pad_00[0x2C];
    short state;
    char pad_2E[0x34 - 0x2E];
    void *def;
    char pad_38[0x70 - 0x38];
    void *vtable;
    char pad_74[0x80 - 0x74];
    Vec4 centre;                /* 0x80 */
    char pad_90[0xA0 - 0x90];
    float radius;
};

extern "C" {

extern int D_089C6CE0;

void func_08807860(SphereTask0880B768 *self, int a, int b, int id, int slot);
void *func_0880BE84(int id);
void func_089A3868(Vec4 *dst, Vec4 *src);

SphereTask0880B768 *func_0880B768(SphereTask0880B768 *self, int a, int b, int id, Vec4 *centre, float radius)
{
    func_08807860(self, a, b, id, -1);
    self->vtable = &D_089C6CE0;
    Vec4 *c = &self->centre;
    self->def = func_0880BE84(id);
    func_089A3868(c, centre);
    self->centre.w = 1.0f;
    self->state = 3;
    self->radius = radius;
    return self;
}

}
