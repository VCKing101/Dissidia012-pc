// With the handle at *self: when flag is 1 and the word at +4 is set, clears bit 14 of its flags short at +0x20; otherwise sets it; then calls func_088D9D24 on the handle.
extern void func_088D9D24(void *);
typedef struct { char pad[0x20]; unsigned short flags; } Handle08B41830;
typedef struct { Handle08B41830 *h; int w4; } Obj08B41830;
void func_08B41830(Obj08B41830 *self, unsigned char flag)
{
    Handle08B41830 *h = self->h;
    if (h != 0) {
        if (flag == 1) {
            if (self->w4 != 0) {
                h->flags &= ~0x4000;
                func_088D9D24(h);
            }
        } else {
            h->flags = h->flags | 0x4000;
            func_088D9D24(h);
        }
    }
}
