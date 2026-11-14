#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): plays the opening talk event the first time;
   when it returns, reads the chosen course and starts it, or goes back to
   the start menu. */
extern "C" {
extern const char D_089E9C94[];
extern void *D_089D6A10;
extern int func_0890113C(int *out, int count);
extern int func_08901568(int *out, int count, int id, int arg);
extern void func_089008F0(void *object);
extern void func_0888964C(void *object, int arg1, int arg2, int arg3, int arg4);
extern void func_08889808(void *object);
extern void func_089E94C0(void);


void func_089E7BD0(ScreenE9D38 *screen)
{
    ScreenE9D38Ex *self = (ScreenE9D38Ex *)screen;
    Story29 *story = SAVE_STORY(D_089D712C);

    func_089E7E60(self);
    if (story->flags & 1) {
        StartMenu29::Open();
    } else if (self->field_190 == 0) {
        self->field_190 = 1;
        func_088E9958(0);
        func_088E980C(D_089E9C94, 0);
        func_088E991C(0, func_089E94C0);
    } else {
        int course[4];
        int stage[4];
        int ok = func_0890113C(course, 4);
        self->field_190 = 0;
        if (ok != 0) {
            int id = course[0];
            if (func_08901568(stage, 4, id, 0) != 0) {
                Global9D8140 *game = D_089D8140;
                void *sound = D_089D6A10;
                game->field_1C |= 1;
                func_08900C04(game, id, 0, 0);
                *(int *)((char *)game + 0xC) = stage[0];
                func_089008F0(game);
                func_0888964C(sound, 10, 0, 0, 0);
                func_08889808(sound);
            } else {
                StartMenu29::Open();
            }
        } else {
            StartMenu29::Open();
        }
    }
}
}
