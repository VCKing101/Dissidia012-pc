#include "main/agent21_types.h"

extern "C" {
extern void func_089055A8(void *self);
extern void func_08993560(void *array, int count, int size, void (*ctor)(void *));
extern void func_0890C35C(void *slot);
extern void func_0890C334(void *item);

struct BoardInit {
    static Board *construct(Board *self)
    {
        func_089055A8(self);
        self->flags.word = 0;
        self->field_14 = 0;
        self->field_18 = 0;
        self->field_40 = 0;
        self->selected = -1;
        self->time = 0;
        func_08993560(self->slots, 2, sizeof(BoardSlot), func_0890C35C);
        func_08993560(self->items, 8, sizeof(BoardItem), func_0890C334);
        self->slotCount = 0;
        self->itemCount = 0;
        self->flags.word = 0;
        return self;
    }
};

Board *func_0890C384(Board *self)
{
    return BoardInit::construct(self);
}
}
