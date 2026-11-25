#include "menu/agent36_types.h"

/* Clock: remakes the time text (0, minutes, seconds of time) and its
 * sprite; keeps the seconds shown. */

extern const char D_08A38FF8[];

void func_08A35EE8(ClockScreen36 *self, unsigned int time)
{
    char *lines[3];
    int count = 3;
    unsigned int minutes = time / 60;
    unsigned int seconds = time % 60;

    lines[0] = self->work->lines[0];
    char *l1 = self->work->lines[1];
    char *l2 = self->work->lines[2]; lines[1] = l1; lines[2] = l2;
    func_089347E8(self->work->lines[0], D_08A38FF8, 0);
    func_089347E8(self->work->lines[1], D_08A38FF8, minutes);
    func_089347E8(self->work->lines[2], D_08A38FF8, seconds);
    if (self->work->texts[CLOCK_TEXT] != 0) {
        func_0887BA5C(self->work->texts[CLOCK_TEXT]);
        self->work->texts[CLOCK_TEXT] = 0;
    }
    if (self->work->texts[CLOCK_SPRITE] != 0) {
        func_0887BA5C(self->work->texts[CLOCK_SPRITE]);
        self->work->texts[CLOCK_SPRITE] = 0;
    }
    self->work->texts[CLOCK_TEXT] = func_08A412A8(D_08A457EC, 0x38, lines, count);
    self->work->texts[CLOCK_SPRITE] = func_0887E534(self->work->texts[CLOCK_TEXT]);
    self->work->shown = seconds;
}
