#include "main/agent2_block_heap.h"

extern int func_08867780(Agent2BlockHeap *, int);
extern unsigned int func_088677C0(Agent2BlockHeap *, unsigned int);

void func_0886734C(Agent2BlockHeap *self) {
    int end;
    self->field_000C = (Agent2Block *)func_088677C0(self, self->field_0000);
    self->field_000C->field_0000 = 0;
    end = self->field_0000 + self->field_0004;
    self->field_000C->field_0004 =
        end - ((int)self->field_000C + func_08867780(self, 0x14));
    self->field_000C->field_0008 = 0;
    self->field_000C->field_000C = 0;
    self->field_000C->field_0010 = 0;
    self->field_0010 = 1;
}
