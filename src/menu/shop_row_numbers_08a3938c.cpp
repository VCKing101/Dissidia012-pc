#include "menu/agent36_types.h"

/* Item list screen: when row's first entry changed, remakes its eight
 * number texts (entries not yet owned and not marked always). */

struct Row3938C {
    char pad_00[0x3];
    unsigned char always;     /* +0x03 */
};

extern "C" {
extern const char D_08A3BE2C[];
Row3938C *func_08A3C1C0(int index);
}

struct NumberText {
    static void *Make(const char *text) { return func_0887D868(D_089D6998, 0, text, -1, 0, 0, 0, 0); }
};

extern "C" void func_08A3938C(ShopScreen36 *self, int row)
{
    int base = row * 8;
    unsigned int first = (row + self->work->cursor.top) * 8;
    int i;

    if (self->work->shown[base] != first) {
        self->work->shown[base] = first;
        for (i = 0; i < 8; i++) {
            if (self->work->texts[base + i] != 0) {
                func_0887BA5C(self->work->texts[base + i]);
                self->work->texts[base + i] = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            int index = i + first;
            if (index >= self->work->list_count) {
                break;
            }
            unsigned short id = self->work->list[index];
            if (func_088CC3B0(id) == 0 && func_08A3C1C0(id)->always == 0) {
                func_089347E8(self->work->number, D_08A3BE2C, id + 1);
                self->work->texts[base + i] = NumberText::Make(self->work->number);
            }
        }
    }
}
