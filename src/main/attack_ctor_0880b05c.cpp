#include "main/agent30_types.h"

/* Attack task constructor: builds the base task, then sets the attack's
   source, its two positions from the given point and a zero offset. */
struct Attack0880B05C {
    char pad_00[0x10];
    unsigned int flags;
    char pad_14[0x2C - 0x14];
    short state;
    char pad_2E[0x70 - 0x2E];
    void *vtable;
    char pad_74[0x80 - 0x74];
    Vec4 from;                  /* 0x80 */
    Vec4 to;                    /* 0x90 */
    Vec4 offset;                /* 0xA0 */
    char pad_B0[0xBC - 0xB0];
    void *source;
    int hits;
};

extern "C" {

extern int D_089C6CA8;

void func_08807860(Attack0880B05C *self, int a, int b, int c, int slot);
void func_089A3868(Vec4 *dst, Vec4 *src);
void func_089A383C(Vec4 *dst, float x, float y, float z, float w);

Attack0880B05C *func_0880B05C(Attack0880B05C *self, int a, int b, int c, void *source, Vec4 *pos, unsigned char pierce)
{
    func_08807860(self, a, b, c, -1);
    self->vtable = &D_089C6CA8;
    Vec4 *from = &self->from;
    Vec4 *to = &self->to;
    Vec4 *offset = &self->offset;
    self->source = source;
    self->hits = 0;
    func_089A3868(to, pos);
    self->to.w = 1.0f;
    func_089A3868(from, pos);
    self->from.w = 1.0f;
    func_089A383C(offset, 0.0f, 0.0f, 0.0f, 0.0f);
    self->state = 2;
    if (pierce) {
        self->flags |= 0x100;
    }
    return self;
}

}
