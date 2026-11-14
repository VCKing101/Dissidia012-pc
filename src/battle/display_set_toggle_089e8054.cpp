#include "battle/agent28_types.h"

/* Forward a show, hide, pause or resume request to both fighter displays of
 * the display set. */
extern "C" {

extern int func_089E567C(FighterDisplay *display);
extern int func_089E5698(FighterDisplay *display);
extern int func_089E56B4(FighterDisplay *display);
extern int func_089E56D0(FighterDisplay *display);

int func_089E8054(DisplaySet *set)
{
    if (!set->open) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089E567C(&set->displays[i]) != 0) {
            return -2;
        }
    }
    return 0;
}

int func_089E80C8(DisplaySet *set)
{
    if (!set->open) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089E5698(&set->displays[i]) != 0) {
            return -2;
        }
    }
    return 0;
}

int func_089E813C(DisplaySet *set)
{
    if (!set->open) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089E56B4(&set->displays[i]) != 0) {
            return -2;
        }
    }
    return 0;
}

int func_089E81B0(DisplaySet *set)
{
    if (!set->open) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        if (func_089E56D0(&set->displays[i]) != 0) {
            return -2;
        }
    }
    return 0;
}

}
