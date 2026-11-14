#include "menu/agent29_types.h"

/* ScreenE9D38 (arcade menu): back out of the course list: closes it and
   the music, resets the game object and returns to the stream or to the
   arcade menu's first page. Retail keeps `self` in the frame. */
extern "C" {
extern unsigned char D_089D5F1F;
extern int D_089E09C0;
void func_089EA0A4(void *list);
void func_089E9FE4(void *list);
void func_089009D4(void *game);
void func_089DE910(int handle, int value);
void func_089DE94C(int handle);

void func_089E91F4(ScreenE9D38 *screen)
{
    Kept<ScreenE9D38Ex *> saved((ScreenE9D38Ex *)screen);
    Global9D8140 *game = D_089D8140;

    func_089EA0A4(saved.value->field_144);
    func_089E9FE4(saved.value->field_144);
    func_088D5528(&saved.value->sound, saved.value->heap);
    func_089009D4(game);
    if (D_089D5F1F != 0) {
        func_089E7E60(saved.value);
        func_089DE910(D_089E09C0, 0);
        func_089DE94C(D_089E09C0);
    } else {
        func_089E7E7C((ScreenE9D38 *)saved.value, 0);
    }
}
}
