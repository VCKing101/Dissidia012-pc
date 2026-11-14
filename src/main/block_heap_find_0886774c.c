#include "main/agent2_block_heap.h"

Agent2Block *func_0886774C(Agent2BlockHeap *self, int id) {
    Agent2Block *block = self->field_000C;
    while (block) {
        if (block->field_0000 == id) return block;
        block = block->field_0010;
    }
    return 0;
}

int func_08867780(Agent2BlockHeap *self, int size) {
    int result = self->field_0008 * (size / self->field_0008);
    if (size % self->field_0008) result += self->field_0008;
    return result;
}

unsigned int func_088677C0(Agent2BlockHeap *self, unsigned int size) {
    unsigned int result =
        self->field_0008 * (size / (unsigned int)self->field_0008);
    if (size % (unsigned int)self->field_0008) result += self->field_0008;
    return result;
}
