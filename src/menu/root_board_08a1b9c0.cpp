#include "menu/agent29_types.h"

/* MenuRoot: releases loader group 9, allocates the 0xC8000-byte board
   block, starts the board object in D_08B01CF8 with it and resumes the
   task. */
extern "C" {
extern void *D_089D7830;
extern void *D_08B01CF8;
void func_088D0B2C(void *loader, int group);
void func_08AF2CB0(void *board);
void func_08AF3130(void *board, int arg1, int arg2, void (*done)(void), void *block, int arg5,
                   int arg6, int arg7, int arg8, int arg9, int arg10);
void func_08A20664(void);
void func_08865184(MenuTask *task);

void func_08A1B9C0(MenuRoot *self)
{
    func_088D0B2C(D_089D7830, 9);
    WORK_INT(self->work, 0x695C) = (int)Heap29::Alloc(self->heap, 0xC8000, true);
    func_08AF2CB0(D_08B01CF8);
    func_08AF3130(D_08B01CF8, 1, 2, func_08A20664, (void *)WORK_INT(self->work, 0x695C), 1, 1, 0, 0,
                  0, 0);
    func_08865184((MenuTask *)&self->pad_00[0x100]);
}
}
