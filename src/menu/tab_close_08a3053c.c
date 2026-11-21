#include "menu/agent36_types.h"

/* Tabbed pages: closes the page: frees its texts and models and puts the
 * camera values back. */
void func_08A3053C(TabScreen36 *self)
{
    int i;
    void *camera;
    int page;

    page = D_08A35878[self->work->tab].field_10;
    func_08A41818(D_08A457EC, page);
    for (i = 0; i < 0x16; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    for (i = 0; i < 0x2C; i++) {
        if (self->work->handles[i] != 0) {
            func_088DA060(self->work->handles[i]);
            self->work->handles[i] = 0;
        }
    }
    func_08A418E8(D_08A457EC);
    camera = D_089D5EC0->camera;
    func_0887358C(camera, self->work->view_6C0);
    func_08873594(camera, self->work->view_6C4, self->work->view_6C8);
    func_088735BC(camera, self->work->view_6CC);
    self->work->flags = 0;
    self->work->field_6A8 = 10;
}
