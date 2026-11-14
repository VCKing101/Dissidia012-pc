#include "menu/agent29_types.h"

/* ScreenE75C4: hides both texts of every entry, then shows the text of
   the current row's entry. */
extern "C" void func_089E2930(ScreenE75C4 *self)
{
    unsigned short index;
    unsigned short text;

    for (int i = 0; i < 0x12; i++) {
        unsigned short off = D_089E7488[i].text_off;
        if (D_089E7488[i].text_on != 0xFFFF) {
            func_0887B944(self->work->texts[D_089E7488[i].text_on]);
        }
        if (off != 0xFFFF) {
            func_0887B944(self->work->texts[off]);
        }
    }
    index = D_089E7518[WORK_INT(self->work, 0xA2C)].entry;
    text = D_089E7488[index].text_on;
    if (D_089E7488[index].enabled == 0) {
        text = D_089E7488[index].text_off;
    }
    func_0887B924(self->work->texts[text]);
}
