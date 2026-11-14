#include "battle/agent1_types.h"

/* Copies the bound model's attachment matrices and colours onto every bound
 * part, or releases the parts once the model is hidden. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct Matrix44 {
    float m[16];
};

struct Rgba {
    unsigned char r, g, b, a;
};

extern "C" {

extern int D_089E4480[2][4][2];
extern void func_088DA0A8(void *model, int name, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(void *part, Matrix44 *matrix);
extern void func_088D9AF4(void *part, Rgba *color);
extern int func_089E439C(BattleBindingSet *set);

int func_089E4060(BattleBindingSet *set)
{
    if (!set->field_00) {
        return -1;
    }
    if (set->field_0C != 0) {
        if (!BoundModel::hidden((BoundModel *)set->field_0C)) {
            if ((unsigned int)set->field_08 >= 2) {
                return -2;
            } else {
                Matrix44 matrix;
                Rgba color;
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 2; j++) {
                        if (set->field_10[i][j] == 0) {
                            continue;
                        }
                        func_088DA0A8(set->field_0C, D_089E4480[set->field_08][i][j], &matrix, &color);
                        func_088D9B84(set->field_10[i][j], &matrix);
                        func_088D9AF4(set->field_10[i][j], &color);
                    }
                }
            }
        } else {
            func_089E439C(set);
        }
    }
    return 0;
}

}
