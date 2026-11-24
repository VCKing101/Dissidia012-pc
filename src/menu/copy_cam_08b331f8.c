/* Releases self->4 through func_088825D4(self->0, self->4) if set, then copies the three words at +0xB8/+0xBC/+0xC0 to D_089D5EC0+0x8B0..0x8B8. */
struct Self { int a; int b; char pad[0xB8 - 8]; float x, y; int z; };
struct Dst { char pad[0x8B0]; float x, y; int z; };
extern struct Dst *D_089D5EC0;
extern void func_088825D4(int a, int b);
void func_08B331F8(struct Self *self)
{
    if (self->b != 0) {
        func_088825D4(self->a, self->b);
        self->b = 0;
    }
    D_089D5EC0->x = self->x;
    D_089D5EC0->y = self->y;
    D_089D5EC0->z = self->z;
}
