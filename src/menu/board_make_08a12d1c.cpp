#include "menu/agent29_types.h"

/* Screen15A88: makes the board's 2 MB heap ("DICOVER") from the shared
   heap and builds the board in a block of it, from the screen's data at
   +0xF64. */
extern "C" {
extern const char D_08A15ED0[];
void *func_0885C40C(void *buffer, int size, const char *name, int align);
void func_08A28B74(Board *board, void *data);
void func_08A28C08(Board *board, void *heap);
}

struct BoardMade29 : Board {
    static void *operator new(unsigned size, void *place) { return place; }
    BoardMade29(void *data) { func_08A28B74(this, data); }
};

extern "C" void func_08A12D1C(Screen15A88 *self)
{
    int size = 0x200000;

    self->work->field_EF8 = Heap29::Alloc(D_089D5EC0->field_710, size, true);
    self->work->field_EFC = func_0885C40C(self->work->field_EF8, size, D_08A15ED0, 0x40);
    self->work->field_EC8 = Heap29::Alloc(self->work->field_EFC, 0x6B408, true);
    self->work->board = new (self->work->field_EC8) BoardMade29((char *)self + 0xF64);
    func_08A28C08(self->work->board, self->work->field_EFC);
}
