#include "menu/agent29_types.h"

/* ScreenE75C4: rebuilds the title text (texts[D_089E6E0E]) from the
   string at D_089D712C +0x1FE08 and places it with script D_089E6DFE. */
extern unsigned short D_089E6E0E;
extern const unsigned short D_089E6DFE[];
void *func_0887D868(void *owner, int arg1, const char *text, int arg3, int arg4, int arg5, int arg6,
                    int arg7);

void func_089E2864(ScreenE75C4 *self)
{
    if (self->work->texts[D_089E6E0E] != 0) {
        func_0887BA5C(self->work->texts[D_089E6E0E]);
        self->work->texts[D_089E6E0E] = 0;
    }
    self->work->texts[D_089E6E0E] =
        func_0887D868(D_089D6998, 0, (const char *)D_089D712C + 0x1FE08, -1, 0, 0, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_089E6DFE, 1);
}
