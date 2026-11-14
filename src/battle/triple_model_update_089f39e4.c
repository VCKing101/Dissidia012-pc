#include "battle/agent28_types.h"

/* Attaches the fill and cap models of a triple-model widget to the base
 * model's bone for the owner's side. */
typedef struct Matrix44 {
    float m[16];
} Matrix44;

typedef struct Rgba {
    unsigned char r, g, b, a;
} Rgba;

extern unsigned int func_08A22CA8(int owner);
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern int D_089F3E7C[2];

int func_089F39E4(TripleModel *widget)
{
    if (!widget->open) {
        return -1;
    }
    if (widget->base != 0 && widget->fill != 0 && widget->cap != 0) {
        unsigned int side = func_08A22CA8(widget->owner);
        Matrix44 matrix;
        Rgba color;

        if (side >= 2) {
            return -2;
        }
        func_088DA0A8(widget->base, D_089F3E7C[side], &matrix, &color);
        if (widget->cap->motion == 9) {
            int bone;

            func_088D9B84(widget->fill, &matrix);
            func_088D9AF4(widget->fill, &color);
            switch (widget->fill->motion) {
            case 3:
                bone = 0;
                break;
            case 4:
                bone = 0;
                break;
            case 5:
                bone = 0;
                break;
            case 6:
                bone = 0;
                break;
            default:
                bone = 0;
                break;
            }
            func_088DA0A8(widget->fill, bone, &matrix, &color);
        }
        func_088D9B84(widget->cap, &matrix);
        func_088D9AF4(widget->cap, &color);
    }
    return 0;
}
