#include "main/agent2_block_heap.h"

extern void func_0886734C(Agent2BlockHeap *);

Agent2BlockHeap *func_088672F4(Agent2BlockHeap *self) {
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000C = 0;
    self->field_0010 = 0;
    return self;
}

int func_08867310(Agent2BlockHeap *self, int base, int size, int align) {
    self->field_0000 = base;
    self->field_0004 = size;
    self->field_0008 = align;
    func_0886734C(self);
    return 1;
}

int func_08867338(Agent2BlockHeap *self) {
    int base = self->field_0000;
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_000C = 0;
    return base;
}
