#include "menu/agent29_types.h"

/* Screen0B6A0: opens the slot counter: counts the filled slots
   (+0x31DC, up to the first empty one of three), plays the full-slots
   motions when all three are filled, shows the counter and loads the
   current entry's item picture and motion; then places the marks. */
extern "C" {
extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short picture);
unsigned short func_089EF83C(Screen0B6A0 *self, signed char index);
unsigned short func_089EF7E8(Screen0B6A0 *self, signed char index, unsigned char second);
void func_089F70F0(Screen0B6A0 *self);
}

/* Inlined calls on the loader in D_089D7830. */
struct Loader9D7830 {
    static void Picture(void *entry, char kind, int picture) {
        func_088D0984(D_089D7830, entry, kind, picture);
    }
};

extern "C" void func_089F6F88(Screen0B6A0 *self)
{
    int i;
    int current;
    int picture;
    int motion;

    WORK_BYTE(self->work, 0x31DC) = 0;
    for (i = 0; i < 3; i++) {
        if (self->work->slots[i].field_00 == -1) {
            break;
        }
        WORK_BYTE(self->work, 0x31DC)++;
    }
    if (WORK_BYTE(self->work, 0x31DC) >= 3) {
        func_088D9F5C(self->work->handles[0xE7], 0x9B, 1);
        func_088D9F5C(self->work->handles[0xE8], 0x9A, 1);
        func_088D9F5C(self->work->handles[0xE9], 0x9A, 1);
    }
    func_088D9F5C(self->work->handles[0xEA], 0x99, 1);
    func_088D9CD8(self->work->handles[0xEE]);
    current = func_089F2FF0(self);
    picture = func_089EF83C(self, self->work->entries[current].field_01);
    Loader9D7830::Picture(self->work->layout.entries[0xA], 0xE, picture);
    motion = func_089EF7E8(self, self->work->entries[current].field_01, 0);
    func_088D9F5C(self->work->handles[0xF2], motion, 1);
    self->work->flags |= 2;
    func_089F70F0(self);
}
