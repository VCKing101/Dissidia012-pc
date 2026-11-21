#include "menu/agent36_types.h"

typedef struct Work4C610 {
    unsigned int flags;       /* +0x0000 */
    int state;                /* +0x0004 */
    char pad_0008[0x6FD0];
    void *list;               /* +0x6FD8 */
    char pad_6FDC[0x10];
    void *slider;             /* +0x6FEC */
    char pad_6FF0[0x4];
    void *names;              /* +0x6FF4 */
} Work4C610;

typedef struct Screen4C610 {
    char pad_00[0x44];
    int page;                 /* +0x44 */
    char pad_48[0x14];
    Work4C610 *work;          /* +0x5C */
} Screen4C610;

void func_08A4CFD4(Screen4C610 *self, float value);
void func_08A4D79C(Screen4C610 *self, unsigned char a);
void func_08A4D868(Screen4C610 *self, int a);
void func_08A6225C(void *list, int a, unsigned char b);
void func_08A66A20(void *slider, int value);
void func_08A5E188(void *names, int value);

void func_08A4C610(Screen4C610 *self)
{
    switch (self->page) {
    case 0:
        func_08A4CFD4(self, 0.0f);
        func_08A4D79C(self, 1);
        self->work->state = 1;
        break;
    case 1:
        self->work->flags |= 0x20;
        func_08A4D868(self, 0);
        func_08A6225C(self->work->list, D_09B6DCE8.field_00, D_09B6DCE8.field_10);
        break;
    case 2:
        self->work->flags |= 0x20;
        func_08A4D868(self, 0);
        func_08A6225C(self->work->list, D_09B6DCE8.field_00, D_09B6DCE8.field_10);
        func_08A66A20(self->work->slider, D_09B6DCE8.field_08);
        func_08A5E188(self->work->names, D_09B6DCE8.field_04);
        break;
    }
}
