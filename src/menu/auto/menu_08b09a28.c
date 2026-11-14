#include "menu/auto_region4.h"

extern const short D_08B1BB8C[];
extern const unsigned short D_08B1BD2C[];
extern const unsigned short D_08B1BD5C[];
extern const unsigned short D_08B1BD74[];
extern const unsigned short D_08B1C5D8[];

/* ScreenB1CB00: plays the opening scripts and shows every part. */
void func_08B09A28(ScreenB1CB00 *self)
{
    unsigned char shown;
    WorkB1CB00 *work;
    MenuLayout *layout;
    MenuModel **handles;

    func_088D3F00(&self->work->layout, self->work->handles, D_08B1BB8C);
    shown = func_08B0DF10(self);
    work = self->work;
    layout = &work->layout;
    handles = work->handles;
    if (shown == 0) {
        func_08AF3930(D_08B01CF8, layout, handles);
        work = self->work;
        layout = &work->layout;
        handles = work->handles;
    }
    func_08AF7654(layout, handles, work->field_1F8AC);
    func_088D44EC(self->work->handles, self->work->texts, D_08B1C5D8);
    func_088D44EC(self->work->handles, self->work->texts, D_08B1BD2C);
    /* handles (+0x404) and texts (+0x884) as byte offsets: the member
     * addresses of this reassigned local cost an extra register copy */
    work = self->work;
    if (work->field_1FA1C != -1) {
        func_088D44EC((char *)work + 0x404, (char *)work + 0x884, D_08B1BD5C);
        work = self->work;
    }
    if (work->field_1FA20 != -1) {
        func_088D44EC((char *)work + 0x404, (char *)work + 0x884, D_08B1BD74);
    }
    func_08B07944(self);
    func_08B052B8(self);
    func_08B069A8(self);
    func_08B06FDC(self);
    func_08B0909C(self);
    func_08B072D4(self);
}
