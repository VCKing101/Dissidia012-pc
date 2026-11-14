#include "menu/agent29_types.h"

/* ScreenE75C4: loads the reward table file (the arcade one when
   D_089D5F20 is set) into a block from the screen's heap and builds the
   table object over it. */

extern "C" {
extern unsigned char D_089D5F20;
extern const char D_089E72A8[];
extern const char D_089E72D4[];
int func_088FE7C8(const char *name, int arg);
void func_088FE708(const char *name, void *block, int size, int arg);
void func_088CAF64(void *table);
void func_088CAF74(void *table, void *data, int size);
void func_088CB00C(void *table);
}

struct RewardTable29 {
    char data[8];
    static void *operator new(unsigned size, void *place) { return place; }
    RewardTable29() { func_088CAF64(this); }
};

extern "C" void func_089E3928(ScreenE75C4 *self)
{
    const char *name = D_089E72A8;
    if (D_089D5F20 != 0) {
        name = D_089E72D4;
    }
    int size = func_088FE7C8(name, 0);
    self->work->field_F80 = Heap29::Alloc(self->heap, size, true);
    func_088FE708(name, self->work->field_F80, size, 0);
    self->work->field_F84 = Heap29::Alloc(self->heap, 8, true);
    self->work->field_F84 = new (self->work->field_F84) RewardTable29;
    func_088CAF74(self->work->field_F84, self->work->field_F80, size);
    func_088CB00C(self->work->field_F84);
}
