#include "battle/agent28_types.h"

/* While the owner plays the prize motion, sets both digit models on the
 * owner's bones; otherwise clears the prize. */
typedef struct Matrix44 {
    float m[16];
} Matrix44;

typedef struct Rgba {
    unsigned char r, g, b, a;
} Rgba;

extern PrizeModel *func_08A01AC0(void *owner);
extern void func_088DA0A8(PrizeModel *model, int bone, Matrix44 *matrix, Rgba *color);
extern void func_088D9B84(PrizeModel *model, Matrix44 *matrix);
extern void func_088D9AF4(PrizeModel *model, Rgba *color);
extern int func_08A191E8(BonusPrize *prize);

int func_08A18F00(BonusPrize *prize)
{
    if (!prize->open) {
        return -1;
    }
    if (prize->shown == 1) {
        PrizeModel *body = func_08A01AC0(prize->owner);
        int playing = 0;
        if (body != 0 && prize->motion >= 0 && body->frame == prize->motion) {
            playing = 1;
        }
        if ((unsigned char)playing == 1 && prize->bones != 0) {
            Matrix44 matrix;
            Rgba color;
            int i;
            for (i = 0; i < 2; i++) {
                if (prize->models[i] == 0) {
                    continue;
                }
                func_088DA0A8(body, prize->bones[i], &matrix, &color);
                func_088D9B84(prize->models[i], &matrix);
                func_088D9AF4(prize->models[i], &color);
            }
        } else {
            func_08A191E8(prize);
        }
    }
    return 0;
}
