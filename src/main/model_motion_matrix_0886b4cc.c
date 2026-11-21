#include "main/agent33_texture.h"

typedef struct Vec4_0886B4CC {
    float x, y, z, w;
} Vec4_0886B4CC;

typedef struct Mat4_0886B4CC {
    Vec4_0886B4CC rows[4];
} Mat4_0886B4CC;

extern void *func_089B0E78(void *model, int motion);             /* the model's motion */
extern void *func_089B1224(void *motion);
extern void func_089A3EFC(Mat4_0886B4CC *out, void *src);

/* The current matrix of a motion of the holder's model. */
void func_0886B4CC(Mat4_0886B4CC *out, D012ModelHolder *holder, int motion)
{
    Mat4_0886B4CC m;

    func_089A3EFC(&m, func_089B1224(func_089B0E78(holder->current_part_addr->model_addr, motion)));
    *out = m;
}
