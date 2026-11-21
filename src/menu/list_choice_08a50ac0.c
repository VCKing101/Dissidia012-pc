#include "menu/agent36_types.h"

typedef struct WorkAC0 {
    char pad_0000[0x4];
    int state;                /* +0x0004 */
    char field_0008[0x6B40];  /* +0x0008 */
    void *list;               /* +0x6B48 */
    char field_6B4C[0xE54];   /* +0x6B4C */
    int field_79A0;           /* +0x79A0 */
} WorkAC0;

typedef struct ScreenAC0 {
    char pad_00[0x58];
    WorkAC0 *work;            /* +0x58 */
} ScreenAC0;

int func_08A69FF4(void *list);
int func_08A6A004(void *list);
int func_08A69FFC(void *list);
void func_08A515F0(ScreenAC0 *self);
void func_08A50740(ScreenAC0 *self);
void func_08A51550(ScreenAC0 *self, void *a, void *b);
void func_08A510E0(ScreenAC0 *self, int index);
void func_08A5112C(ScreenAC0 *self, int index);

void func_08A50AC0(ScreenAC0 *self)
{
    switch (func_08A69FF4(self->work->list)) {
    case 0:
        break;
    case 1:
        func_08A515F0(self);
        break;
    case 2:
        func_08A50740(self);
        self->work->state = 0;
        break;
    case 3:
        func_08A51550(self, self->work->field_0008, self->work->field_6B4C);
        func_08A50740(self);
        self->work->state = 6;
        break;
    case 4:
        func_08A510E0(self, func_08A6A004(self->work->list));
        self->work->field_79A0 = 0;
        func_08A50740(self);
        self->work->state = 0xB;
        break;
    case 5:
        func_08A5112C(self, func_08A69FFC(self->work->list));
        self->work->field_79A0 = 0;
        func_08A50740(self);
        self->work->state = 0xB;
        break;
    }
}
