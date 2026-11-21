#include "menu/agent36_types.h"

/* Tabbed pages: places the item mark (handle 0x26 of the battle table)
 * at handle 0x1F moved by the current record's offset for item a or b. */
extern "C" void func_08A2FC20(TabScreen36 *self)
{
    float matrix[16];
    int color;
    void **handles;
    TabRecord36 *record;

    if (self->work->tab == 0) {
        return;
    }
    handles = D_08A457EC->table->handles;
    func_088DA0A8(handles[0x1F], 3, matrix, &color);
    record = func_08A3C164(self->work->ids[1][self->work->cursor.index]);
    if (self->work->field_6D7 == 0) {
        if (record->x_a != 0) {
            matrix[12] += record->x_a;
        }
        if (record->y_a != 0) {
            matrix[13] += record->y_a;
        }
    } else {
        if (record->x_b != 0) {
            matrix[12] += record->x_b;
        }
        if (record->y_b != 0) {
            matrix[13] += record->y_b;
        }
    }
    func_088D9B84(handles[0x26], matrix);
    func_088D9AF4(handles[0x26], &color);
}
