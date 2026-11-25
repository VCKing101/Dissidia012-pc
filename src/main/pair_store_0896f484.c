/* Stores two 0x40-byte blocks (from a2 at 0xB0, from a1 at 0xF0), derives the block at 0x170 from the first, then refreshes the object. */
typedef struct B0896F484 { int w[16]; } B0896F484;
typedef struct O0896F484 { char pad[0xB0]; B0896F484 x; B0896F484 y; B0896F484 gap; B0896F484 z; } O0896F484;
void func_0896F024(B0896F484 *dst, B0896F484 *src);
void func_0896F398(O0896F484 *o);
void func_0896F484(O0896F484 *self, B0896F484 *a, B0896F484 *b)
{
    self->x = *b;
    self->y = *a;
    func_0896F024(&self->z, &self->x);
    func_0896F398(self);
}
