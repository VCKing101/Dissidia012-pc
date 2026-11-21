#include "menu/agent36_types.h"

/* Shop screen pack: makes the pack heap, then reads each pack file (as many
 * as the disc has, up to 16) into a block of its own. */
struct Pack3A8E8 {
    int field_00;
    void *data;               /* +0x04 */
    int size;                 /* +0x08 */
    char path[0x84];          /* +0x0C */
};

struct Packs3A8E8 {
    int count;                /* +0x000 */
    Pack3A8E8 packs[16];      /* +0x004 */
    void *block;              /* +0x904 */
    void *heap;               /* +0x908 */
    unsigned char packed;     /* +0x90C */
};

extern "C" {
extern char D_08A3BE38[];
extern const char *D_08A3BE68;
void *func_0885C6CC(void *heap, int size, int align, int a, int clear, int b);
void *func_0885C40C(void *block, int size, const char *name, int count);
int func_088CC22C(void);
void func_0893476C(char *out, const char *format, int value);
int func_088FE7C8(const char *path, int a);
int func_08862C40(const char *path, int a);
void func_08A3AB98(Packs3A8E8 *self, int index);
}

struct Heap3A8E8 {
    static void *Alloc(void *heap, int size, bool clear)
    {
        return func_0885C6CC(heap, size, 0, 0, clear, 0);
    }
};

extern "C" void func_08A3A8E8(Packs3A8E8 *self)
{
    int size = 0x200000;

    self->block = Heap3A8E8::Alloc(D_089D5EC0->heap, size, true);
    self->heap = func_0885C40C(self->block, size, D_08A3BE38, 0x40);
    self->count = ((func_088CC22C() + 0x1F) & ~0x1F) / 32;
    if (self->count >= 0x10) {
        self->count = 0x10;
    }
    for (int i = 0; i < self->count; i++) {
        func_0893476C(self->packs[i].path, D_08A3BE68, i);
        if (self->packed) {
            self->packs[i].size = func_088FE7C8(self->packs[i].path, 0);
        } else {
            self->packs[i].size = func_08862C40(self->packs[i].path, 1);
        }
        self->packs[i].data = Heap3A8E8::Alloc(self->heap, self->packs[i].size, true);
        func_08A3AB98(self, i);
    }
}
