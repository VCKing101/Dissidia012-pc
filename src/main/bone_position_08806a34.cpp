#include "main/agent30_types.h"

/* World position of the camera's target bone, or the target's own position
   when no bone is set. */
struct Skeleton08806A34;

struct Model08806A34 {
    char pad_0[8];
    Skeleton08806A34 *skeleton;
};

struct Attach08806A34 {
    char pad_00[0x18];
    int bone;

};

struct Target08806A34 {
    char pad_00[0x84];
    Model08806A34 *model;
    char pad_88[0xA4 - 0x88];
    Attach08806A34 attach;
};

struct Node08806A34 {
    float matrix[16];
    char pad_40[0x40];
    Vec4 pos;                   /* 0x80 */
    char pad_90[0x394 - 0x90];
    Target08806A34 *target;
};

extern "C" {

void *func_089B0E78(Skeleton08806A34 *skeleton, int index);
char *func_089B1224(void *bone);
void func_089A3868(Vec4 *dst, void *src);
void func_089A3F40(Vec4 *dst, Node08806A34 *matrix, Vec4 *src);

Vec4 func_08806A34(Node08806A34 *self)
{
    Target08806A34 *t = self->target;
    Model08806A34 *model = t->model;
    Attach08806A34 *a = &t->attach;
    Skeleton08806A34 *skeleton = model->skeleton;
    int bone = a->bone;
    if (bone < 0) {
        return self->pos;
    }
    Vec4 v;
    func_089A3868(&v, func_089B1224(func_089B0E78(skeleton, bone)) + 0x30);
    func_089A3F40(&v, self, &v);
    return v;
}

}
