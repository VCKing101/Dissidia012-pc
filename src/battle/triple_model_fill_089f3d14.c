#include "battle/agent28_types.h"

/* Advances the fill model's motion once the fill time reaches the motion's
 * switch point, then refreshes the widget. */
extern float D_089F3E78;
extern void func_088D9F5C(void *model, int motion, int loop);
extern int func_089F39E4(TripleModel *widget);

float func_089F3D14(TripleModel *widget, float time, float length)
{
    if (!widget->open) {
        return -1.0f;
    }
    if (widget->fill == 0) {
        return -2.0f;
    }
    switch (widget->fill->motion) {
    case 4:
        if (time >= length * 0.5f - D_089F3E78) {
            func_088D9F5C(widget->fill, 5, 1);
            widget->fill->phase = 3;
        }
        break;
    case 3:
        if (time >= length - D_089F3E78) {
            func_088D9F5C(widget->fill, 6, 1);
            widget->fill->phase = 2;
        }
        break;
    }
    func_089F39E4(widget);
    return 0.0f;
}
