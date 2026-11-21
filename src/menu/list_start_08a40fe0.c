#include "menu/agent36_types.h"

/* List screen: starts the list in mode (resetting the list state; -1 keeps
 * the saved option 0x11, 0 restores it) and starts the screen task. */

unsigned char func_088F47E0(int kind);
void func_088F4870(int kind, unsigned char value);
void func_08A40F88(void *self);
void func_088650A4(void *task);

typedef struct ListScreen36 {
    char pad_000[0x100];
    char task[0x34];          /* +0x100 */
} ListScreen36;

void func_08A40FE0(ListScreen36 *self, int mode)
{
    int none;

    D_09B6DA68.mode = mode;
    D_09B6DA68.field_1E = 0;
    D_09B6DA68.field_1F = 0;
    D_09B6DA68.count = 0;
    D_09B6DA68.index = 0;
    none = -1;
    if (mode == none) {
        D_09B6DA68.saved = func_088F47E0(0x11);
        D_09B6DA68.field_20 = 0;
    }
    if (mode == 0) {
        func_088F4870(0x11, D_09B6DA68.saved);
    } else {
        D_09B6DA68.field_0C = 0;
        D_09B6DA68.field_10 = 0;
        D_09B6DA68.field_14 = 0;
        D_09B6DA68.field_18 = 0;
        D_09B6DA68.field_24 = none;
        D_09B6DA68.field_28 = none;
        D_09B6DA68.field_2C = none;
    }
    func_08A40F88(self);
    func_088650A4(self->task);
}
