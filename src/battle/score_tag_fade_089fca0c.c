#include "battle/agent28_types.h"

/* Moves the score tag's fade toward the target and applies it to the model
 * as a clamped frame. */
extern float D_089FCB10;
extern float D_089FCB14;
extern float D_089FCB18;
extern float D_089FCB1C;
extern void func_088D9AEC(void *model, float frame);

int func_089FCA0C(ScoreTag *tag, float target)
{
    void *model;
    float fade;
    float speed;

    if (!tag->open) {
        return -1;
    }
    fade = tag->fade;
    model = tag->model;
    if (fade < target) {
        speed = D_089FCB10;
        fade += speed;
        tag->fade = fade;
        if (fade > target) {
            tag->fade = target;
        }
    } else if (fade > target) {
        fade -= D_089FCB10;
        tag->fade = fade;
        if (fade < target) {
            tag->fade = target;
        }
    }
    if (model != 0) {
        float frame = tag->fade;
        if (frame < D_089FCB14) {
            frame = D_089FCB14;
        } else if (frame > D_089FCB18) {
            frame = D_089FCB18;
        }
        func_088D9AEC(model, D_089FCB1C * frame);
    }
    return 0;
}
