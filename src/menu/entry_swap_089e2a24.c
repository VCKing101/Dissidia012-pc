#include "menu/agent29_types.h"

/* ScreenE75C4: shows the text of entry `shown` and hides the text of
   entry `hidden`, each picked by whether the entry is enabled. */
void func_089E2A24(ScreenE75C4 *self, int shown, int hidden)
{
    unsigned short show = D_089E7488[shown].text_on;
    unsigned short hide = D_089E7488[hidden].text_on;

    if (D_089E7488[shown].enabled == 0) {
        show = D_089E7488[shown].text_off;
    }
    if (D_089E7488[hidden].enabled == 0) {
        hide = D_089E7488[hidden].text_off;
    }
    func_0887B924(self->work->texts[show]);
    func_0887B944(self->work->texts[hide]);
}
