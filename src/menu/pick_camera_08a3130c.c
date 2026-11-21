#include "menu/agent36_types.h"

typedef struct Vec3130C {
    float x;
    float y;
    float z;
    float w;
} Vec3130C;

typedef struct Camera3130C {
    Vec3130C eye;             /* +0x000 */
    Vec3130C at;              /* +0x010 */
    char pad_020[0x12C];
    float field_14C;          /* +0x14C */
    float field_150;          /* +0x150 */
    float field_154;          /* +0x154 */
    float field_158;          /* +0x158 */
    float field_15C;          /* +0x15C */
} Camera3130C;

typedef struct View3130C {
    float field_0;
    float height;             /* +0x4 */
    float distance;           /* +0x8 */
} View3130C;

typedef struct Screen3130C {
    char pad_00[0x48];
    Camera3130C *camera;      /* +0x48 */
} Screen3130C;

void func_089A3868(Vec3130C *dst, const Vec3130C *src);
void func_08A33D8C(Screen3130C *self);

void func_08A3130C(Screen3130C *self, View3130C *view)
{
    Vec3130C eye = { 0.0f, 1.28f, 0.0f, 1.0f };
    Vec3130C at = { 0.0f, 0.0f, 0.0f, 1.0f };

    self->camera->field_154 = view->height;
    func_089A3868(&self->camera->eye, &eye);
    self->camera->eye.y = self->camera->field_154;
    self->camera->field_158 = 0.0f;
    self->camera->field_15C = 0.0f;
    func_089A3868(&self->camera->at, &at);
    self->camera->field_14C = view->distance;
    self->camera->field_150 = 0.0f;
    func_08A33D8C(self);
}
