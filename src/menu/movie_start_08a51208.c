#include "menu/agent36_types.h"

typedef struct Work51208 {
    char pad_0000[0x79A4];
    int kind;                 /* +0x79A4 */
    void *buffer;             /* +0x79A8 */
    char pad_79AC[0xC0];
    char field_7A6C[0x154];   /* +0x7A6C */
    char field_7BC0[0x10];    /* +0x7BC0 */
} Work51208;

typedef struct Screen51208 {
    char pad_00[0x10];
    void *heap;               /* +0x10 */
    char field_14[0x44];      /* +0x14 */
    Work51208 *work;          /* +0x58 */
} Screen51208;

void func_08A504DC(Screen51208 *self);
void *func_0885C6CC(void *heap, int size, int align, int a, int b, int c);
void func_088D0B2C(void *p, int id);
void func_08A510BC(void);
void func_08AF2CB0(void *p);
void func_08AF2D08(void *p, const char *name, int a, void (*done)(void), void *buffer, int b, int c, int d,
                   int e, int f, int g, int h, int i, int j, int k);
void func_08AF3200(void *p, void *data, int a, void (*done)(void), void *buffer, int b);
void func_08AF2F80(void *p);
void func_08AF2F88(void *p, void *data, void (*done)(void), void *buffer, int a, int b, int c);
void func_08865184(void *p);

void func_08A51208(Screen51208 *self, const char *name)
{
    func_08A504DC(self);
    self->work->buffer = func_0885C6CC(self->heap, 0xC8000, 0x40, 0, 1, 0);
    func_088D0B2C(D_089D7830, 0x1A);
    switch (self->work->kind) {
    case 0:
        func_08AF2CB0(D_08B01CF8);
        func_08AF2D08(D_08B01CF8, name, 0, func_08A510BC, self->work->buffer, 1, 1, 1, 0, -1, 0, 0, 0, 0, 0);
        break;
    case 1:
        func_08AF2CB0(D_08B01CF8);
        func_08AF3200(D_08B01CF8, self->work->field_7BC0, 0, func_08A510BC, self->work->buffer, 1);
        break;
    case 2:
        func_08AF2F80(D_08B01CF8);
        func_08AF2F88(D_08B01CF8, self->work->field_7A6C, func_08A510BC, self->work->buffer, 0, 0, 0);
        break;
    }
    func_08865184(self->field_14);
}
