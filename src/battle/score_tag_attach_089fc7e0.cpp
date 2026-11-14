#include "battle/agent28_types.h"

/* Attaches the score tag to its owner model's bone for the side. */
struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

extern "C" {

extern int D_089FCB28[2];
extern unsigned int func_08A22CA8(int owner);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_089FC7E0(ScoreTag *tag)
{
    if (!tag->open) {
        return -1;
    }
    if (tag->model != 0 && tag->tag != 0) {
        unsigned int side = func_08A22CA8(tag->owner);
        if (side >= 2) {
            return -2;
        } else {
            Matrix44 matrix;
            Rgba color;
            func_088DA0A8(tag->model, D_089FCB28[side], &matrix, &color);
            func_088D9B84(tag->tag, &matrix);
            func_088D9AF4(tag->tag, &color);
        }
    }
    return 0;
}

}
