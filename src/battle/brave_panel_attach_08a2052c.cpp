#include "battle/agent28_types.h"

/* Sets the Brave panel's four parts on the panel's bones, faded by the
 * side's Brave fade. */
struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

extern "C" {

extern RootHolder *D_089D6A10;
extern int D_08A20680[2][4];
extern float func_089DDBDC(void *root, unsigned int side);
extern unsigned int func_08A22CA8(unsigned int side);
extern void *func_089F7DDC(void *parts, int index);
extern void func_088DA0A8(void *model, int bone, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);

int func_08A2052C(BravePanel *panel)
{
    if (panel->model != 0) {
        void *root = D_089D6A10->root;
        if (root == 0) {
            return -1;
        }
        float fade = func_089DDBDC(root, panel->side);
        unsigned int kind = func_08A22CA8(panel->side);
        if (kind >= 2) {
            return -2;
        }
        Matrix44 matrix;
        Rgba color;
        for (int i = 0; i < 4; i++) {
            void *part = func_089F7DDC(panel->parts, i);
            if (part == 0) {
                continue;
            }
            func_088DA0A8(panel->model, D_08A20680[kind][i], &matrix, &color);
            color.a = (unsigned char)(int)(color.a * fade);
            func_088D9B84(part, &matrix);
            func_088D9AF4(part, &color);
        }
    }
    return 0;
}

}
