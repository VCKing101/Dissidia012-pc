#include "menu/auto_region4.h"

extern const unsigned short D_08B1C498[];

/* ScreenB1CB00: marks the picked one of the five-row (handles[0xE2..],
 * texts[0x110..]) and remakes texts[0x10A] from its record. */
void func_08B0D710(ScreenB1CB00 *self)
{
    int *count;
    unsigned int *id;
    void *text;
    WorkB1CB00 *work;
    void *table;
    int i;
    RecordBB88C *record;

    i = 0;
    work = self->work;
    do {
        func_088D9F5C(work->handles[i + 0xE2], 0xD, 1);
        func_0887B944(self->work->texts[i + 0x110]);
        i = i + 1;
        work = self->work;
    } while (i < 5);
    func_088D9F5C(work->handles[work->field_24768 + 0xE2], 0xB, 1);
    func_0887B924(self->work->texts[self->work->field_24768 + 0x110]);
    work = self->work;
    if (work->texts[0x10A] != 0) {
        func_0887BA5C(work->texts[0x10A]);
        self->work->texts[0x10A] = 0;
        work = self->work;
    }
    table = ((Global9D712CView *)D_089D712C)->field_3F548;
    i = work->field_24768;
    count = func_089200D8(table);
    work = self->work;
    if (i < *count) {
        id = func_089200E0(table, work->field_24768);
        record = func_088BB88C(*id);
        text = func_0887AD78((void *)0x09B4E848, 0, record->field_0D, 0, 0, 0);
        self->work->texts[0x10A] = text;
        work = self->work;
    }
    /* handles (+0x404) and texts (+0x884) as byte offsets: the member
     * addresses of this reassigned local cost an extra register copy */
    func_088D428C((char *)work + 0x404, (char *)work + 0x884, D_08B1C498, 1);
    i = 0;
    work = self->work;
    do {
        func_0887B944(work->texts[i + 0x110]);
        i = i + 1;
        work = self->work;
    } while (i < 5);
    func_0887B924(work->texts[work->field_24768 + 0x110]);
    func_08B0D8CC(self);
}
