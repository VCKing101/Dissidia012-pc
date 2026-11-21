#include "menu/agent36_types.h"

struct Model39184 {
    char pad_00[0x4];
    int field_04;
    int field_08;
};

extern "C" {
const char *func_08A3C0C4(int id);
Model39184 *func_08A390D8(ShopScreen36 *self, int model);
void func_088DAE0C(void *entry, int a, int b, int c, int d);
}

/* Item list: shows the eight cells of row: hides each cell, then shows the
 * picture of an owned item (setting the cell's model when it changed). */
extern "C" void func_08A39184(ShopScreen36 *self, int row)
{
    int i;
    int top = self->work->cursor.top;
    unsigned int base = row * 8;
    unsigned int first = (row + top) * 8;

    func_08A3C0C4(0);
    for (i = 0; i < 8; i++) {
        int cell = i + base;
        func_088D9CFC(self->work->handles[cell + 0x29]);
        int index = i + first;
        if (index >= self->work->list_count) {
            continue;
        }
        unsigned short id = self->work->list[index];
        if (func_088CC3B0(id) == 0) {
            continue;
        }
        ShopItem36 *item = func_088CC364(id);
        Model39184 *model = func_08A390D8(self, item->model);
        if (model != 0) {
            if (self->work->marks[cell] != item->model) {
                self->work->marks[cell] = item->model;
                func_088DAE0C(self->work->layout[cell + 0x13], model->field_04, model->field_08, -1, -1);
            }
            func_088D9CD8(self->work->handles[cell + 0x29]);
        }
    }
}
