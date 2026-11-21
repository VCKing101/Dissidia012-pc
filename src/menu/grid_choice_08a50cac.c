#include "menu/agent36_types.h"

typedef struct WorkCAC {
    char pad_0000[0x4];
    int state;                /* +0x0004 */
    char pad_0008[0x7990];
    void *grid;               /* +0x7998 */
    char pad_799C[0x4];
    int field_79A0;           /* +0x79A0 */
} WorkCAC;

typedef struct ScreenCAC {
    char pad_00[0x4];
    int mode;                 /* +0x04 */
    char pad_08[0x40];
    char field_48[0x10];      /* +0x48 */
    WorkCAC *work;            /* +0x58 */
} ScreenCAC;

int func_08A6B728(void *grid);
int func_08A6B730(void *grid);
void func_08A51698(ScreenCAC *self);
void func_08A50740(ScreenCAC *self);
void func_088D54DC(void *p);
void func_08A50524(ScreenCAC *self);
void func_08A511B0(ScreenCAC *self, int index);

void func_08A50CAC(ScreenCAC *self)
{
    switch (func_08A6B728(self->work->grid)) {
    case 0:
        break;
    case 1:
        func_08A51698(self);
        break;
    case 3:
        func_08A50740(self);
        func_088D54DC(self->field_48);
        func_08A50524(self);
        self->mode = 1;
        self->work->state = 9;
        break;
    case 2:
        func_08A50740(self);
        self->work->state = 3;
        break;
    case 4:
        func_08A511B0(self, func_08A6B730(self->work->grid));
        self->work->field_79A0 = 1;
        func_08A50740(self);
        self->work->state = 0xB;
        break;
    }
}
