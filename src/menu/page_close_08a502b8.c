#include "menu/agent36_types.h"

typedef struct Work502B8 {
    char pad_0000[0x6B14];
    void *message;            /* +0x6B14 */
    char pad_6B18[0x4];
    void *block_6B1C;         /* +0x6B1C */
    void *block_6B20;         /* +0x6B20 */
    char pad_6B24[0x24];
    void *list;               /* +0x6B48 */
    char pad_6B4C[0xE4C];
    void *grid;               /* +0x7998 */
} Work502B8;

typedef struct Screen502B8 {
    char pad_00[0x4];
    int mode;                 /* +0x04 */
    char pad_08[0x8];
    void *heap;               /* +0x10 */
    char pad_14[0x34];
    char field_48[0x10];      /* +0x48 */
    Work502B8 *work;          /* +0x58 */
} Screen502B8;

extern void *D_089D8530;
void func_088D5528(void *p, void *heap);
void func_08A69C5C(void *list, int a);
void func_08A6B45C(void *grid, int a);
void func_08A504DC(Screen502B8 *self);
void func_0887B1F4(void *message, int a);
void func_088D0370(void *p, int a, int b);
void func_08927648(void *p);
void func_08A50438(Screen502B8 *self);
void func_08A505C0(Screen502B8 *self);
void func_08A50650(Screen502B8 *self);

void func_08A502B8(Screen502B8 *self)
{
    func_088D5528(self->field_48, self->heap);
    func_0885C538(self->heap, self->work->block_6B20, 0);
    func_0885C538(self->heap, self->work->block_6B1C, 0);
    func_08A69C5C(self->work->list, 2);
    func_0885C538(self->heap, self->work->list, 0);
    func_08A6B45C(self->work->grid, 2);
    func_0885C538(self->heap, self->work->grid, 0);
    func_08A504DC(self);
    if (self->work->message != 0) {
        func_0887B1F4(self->work->message, 3);
        self->work->message = 0;
    }
    {
        int on = 1;
        int sound = 0x1A;
        func_088D0370(D_089D7830, sound, on);
    }
    func_08927648(D_089D8530);
    {
        Work502B8 *work = self->work;
        func_0885C538(self->heap, work, 0);
    }
    self->work = 0;
    func_08A50438(self);
    if (self->mode == 1) {
        func_08A505C0(self);
    } else {
        func_08A50650(self);
    }
}
