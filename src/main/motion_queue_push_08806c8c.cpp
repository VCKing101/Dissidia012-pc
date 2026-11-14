#include "main/agent30_types.h"

/* Appends a code to the motion queue's ring buffer. */
extern "C" {

void func_08806C8C(MotionQueue *self, unsigned char code)
{
    unsigned int i = (self->head + self->count) % 64;
    self->codes[i] = code;
    self->count++;
    self->pushed++;
}

}
