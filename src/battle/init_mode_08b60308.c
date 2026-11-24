/* Initialises the object: mode 1 when the flag is set, otherwise 3; clears the byte at +0xC and the float at +0x18. */
struct Obj { int mode; int a; int b; unsigned char c; char pad[0xB]; float f; };
void func_08B60308(struct Obj *self, int flag)
{
    self->mode = flag ? 1 : 3;
    self->c = 0;
    self->f = 0.0f;
}
