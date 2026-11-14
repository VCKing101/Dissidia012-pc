/* When flag 0x1000 of the window is set, switches its layout into page
   mode (`mode` set) or back out of it.
   (Not a stack-spill function.) */
typedef struct Window08ACD0D8 {
    char unknown_000[0xF18];
    unsigned flags;
} Window08ACD0D8;

typedef struct Menu08ACD0D8 {
    char unknown_0000[0xDB30];
    Window08ACD0D8 *window;
} Menu08ACD0D8;

extern char D_08ADA1CA[];
extern char D_08ADA1EA[];
void func_088D3E40(Window08ACD0D8 *window, void *layout, void *name);
void func_088D3EA0(Window08ACD0D8 *window, void *layout, void *name);
void func_088D45E4(void *layout, void *screen, void *name);
void func_088D4664(void *layout, void *screen, void *name);

void func_08ACD0D8(Menu08ACD0D8 *self, unsigned char mode)
{
    if ((self->window->flags & 0x1000) != 0) {
        if (mode != 0) {
            func_088D3E40(self->window, (char *)self->window + 0x400, D_08ADA1CA);
            func_088D45E4((char *)self->window + 0x400, (char *)self->window + 0x880, D_08ADA1EA);
        } else {
            func_088D3EA0(self->window, (char *)self->window + 0x400, D_08ADA1CA);
            func_088D4664((char *)self->window + 0x400, (char *)self->window + 0x880, D_08ADA1EA);
        }
    }
}
