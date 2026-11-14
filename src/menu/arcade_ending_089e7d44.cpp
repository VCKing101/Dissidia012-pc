#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): plays the ending talk event, then marks it
   seen, clears the game flag and goes back to the start menu. Retail
   keeps `self` in the frame across the first call. */
extern "C" {
extern const char D_089E9CB8[];
void func_089E94E8(void);

void func_089E7D44(ScreenE9D38 *screen)
{
    Kept<ScreenE9D38Ex *> saved((ScreenE9D38Ex *)screen);
    func_089E7E60(saved.value);
    ScreenE9D38Ex *self = saved.value;
    if (self->field_190 == 0) {
        self->field_190 = 1;
        func_088E9958(0);
        func_088E980C(D_089E9CB8, 0);
        func_088E991C(0, func_089E94E8);
    } else {
        Story29 *story = SAVE_STORY(D_089D712C);
        story->flags |= 1;
        self->field_190 = 0;
        D_089D8140->field_1C &= ~1;
        StartMenu29::Open();
    }
}
}
