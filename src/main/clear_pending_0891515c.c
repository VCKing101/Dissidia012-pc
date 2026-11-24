/* If the pending byte at 0x7DF is set: when the id at 0x7B0 is non-negative, calls func_08B43AE8 with the object's field 0x88 and a halfword from func_08910644's result; then clears the byte. */
typedef struct Obj0891515c { unsigned char pad0[0x88]; int h; unsigned char pad1[0x7B0 - 0x8C]; int id; unsigned char pad2[0x7DF - 0x7B4]; unsigned char pending; } Obj0891515c;
typedef struct Info0891515c { unsigned char pad[6]; short v; } Info0891515c;
extern Info0891515c *func_08910644(void);
extern void func_08B43AE8(int h, int v);
void func_0891515C(Obj0891515c *self)
{
    if (self->pending) {
        if (self->id >= 0) {
            Info0891515c *info = func_08910644();
            func_08B43AE8(self->h, info->v);
        }
        self->pending = 0;
    }
}
