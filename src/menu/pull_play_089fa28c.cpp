#include "menu/agent29_types.h"

/* Screen0B6A0: once the shared object and the save are idle, starts the
   pull animation: frees the page block, loads resource group 0xE, makes
   the animation's 0xC8000-byte buffer, starts the player D_08B01CF8 on
   the record list with func_089FE5F4 as its callback, and resumes. */
extern "C" {
extern void *D_089D7830;
int func_088FEA4C(void);
void func_0887E684(void *object);
void func_088D0B2C(void *loader, int group);
void func_08865184(MenuTask *task);
void func_089FE5F4(void);
void func_08AF2CB0(void *player);
void func_08AF2D08(void *player, void *list, int count, void (*callback)(void), void *buffer, int arg5, int arg6,
                   int arg7, int arg8, int arg9, int arg10, int arg11, int arg12, int arg13, int arg14);
}

extern "C" void func_089FA28C(Screen0B6A0 *self)
{
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    if (func_088FEA4C() != 0) {
        return;
    }
    func_0887E684(D_089D6998);
    func_0885C538(self->heap, (void *)WORK_INT(self->work, 0), 0);
    WORK_INT(self->work, 0) = 0;
    func_088D0B2C(D_089D7830, 0xE);
    WORK_INT(self->work, 0x3414) = (int)Heap29::Alloc(self->heap, 0xC8000, true);
    WORK_INT(self->work, 0x2E80) = 0;
    func_08AF2CB0(D_08B01CF8);
    func_08AF2D08(D_08B01CF8, &WORK_INT(self->work, 0x2E80), WORK_INT(self->work, 0x33C8C), func_089FE5F4,
                  (void *)WORK_INT(self->work, 0x3414), 1, 1, 1, 0, -1, 0, 0, 0, 0, 0);
    func_08865184(&self->task);
}
