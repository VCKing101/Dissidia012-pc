/* Window setup and teardown for the menu at +0xC18: func_08AC8508 opens it
   (state 1), func_08AC85D8 closes it, releases its 0x15 items and frees its
   buffer (state 4).
   (Not stack-spill functions.) */
typedef struct Window08AC8508 {
    void *layout;
    void *screen;
    int unknown_08;
    void *items[0x15];
    char unknown_060[0x80];
    void *buffer;
    char unknown_0E4[0xAF4];
    int fBD8;
    int fBDC;
    int fBE0;
    char unknown_BE4[0x18];
    int state;
    unsigned flags;
    char unknown_C04[0x10];
    unsigned char kind;
    unsigned char beep;
} Window08AC8508;

typedef struct Menu08AC8508 {
    char unknown_000[0xC18];
    Window08AC8508 *window;
} Menu08AC8508;

typedef struct Globals08AC8508 {
    char unknown_000[0x134];
    void *heap;
} Globals08AC8508;

extern Globals08AC8508 *D_08AC7500;
extern char D_08ACA1B6[];
extern char D_08ACA1D2[];
void func_088D3910(int id, void *layout, void *name, int a, int b);
void func_088D3B88(void *layout, void *screen, void *name, int a, int b);
void func_08AC925C(Menu08AC8508 *menu, unsigned char kind);
void func_088D4BF4(int sound);
void func_08AC5DB4(Globals08AC8508 *globals);
void func_088D3C24(void *layout, void *screen, void *name, int a);
void func_088D3A5C(void *layout, void *name, int a);
void func_0887BA5C(void *item);
void func_088F14D4(void);
void func_0885C538(void *heap, void *data, int flags);

void func_08AC8508(Menu08AC8508 *self)
{
    self->window->fBE0 = 0;
    self->window->fBD8 = 0;
    self->window->fBDC = 0;
    func_088D3910(0xB, self->window->layout, D_08ACA1B6, 8, 1);
    func_088D3B88(self->window->layout, self->window->screen, D_08ACA1D2, 0, 0x120);
    func_08AC925C(self, self->window->kind);
    if (self->window->beep != 0) {
        func_088D4BF4(0xA);
        self->window->beep = 0;
    }
    func_08AC5DB4(D_08AC7500);
    self->window->flags |= 6;
    self->window->state = 1;
}

void func_08AC85D8(Menu08AC8508 *self)
{
    int i;
    Window08AC8508 *window;
    func_088D3C24(self->window->layout, self->window->screen, D_08ACA1D2, 0x120);
    func_088D3A5C(self->window->layout, D_08ACA1B6, 8);
    for (i = 0; i < 0x15; i++) {
        if (self->window->items[i] != 0) {
            func_0887BA5C(self->window->items[i]);
            self->window->items[i] = 0;
        }
    }
    func_088F14D4();
    window = self->window;
    if (window->buffer != 0) {
        func_0885C538(D_08AC7500->heap, window->buffer, 0);
        self->window->buffer = 0;
        window = self->window;
    }
    window->state = 4;
}
