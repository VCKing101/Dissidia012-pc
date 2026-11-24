/* Marks a request started, calls the loader with 0xFFFF, and clears the mark if it fails. */
extern int func_0899C52C(int h, int v);
typedef struct { int h; int pad; unsigned char started; } Req;
void func_08B45380(Req *self)
{
    if (self->started != 0) return;
    self->started = 1;
    if (func_0899C52C(self->h, 0xFFFF) != 0) return;
    self->started = 0;
}
