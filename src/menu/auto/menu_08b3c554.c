#include "menu/auto_region4.h"

extern int D_089D6AC8;
extern int D_09AFB89C;

void func_088421EC(void);
void func_08892DCC(int object);
void func_08893418(int arg1, int arg2, int arg3, int arg4);
void func_08893534(int arg);
void func_0889384C(int object, void *heap);
void func_08893B90(int task, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8);
void func_08893C6C(int task, int arg1, int arg2);
void func_08893C94(int object, int arg);
void func_08898DF8(int object, void *heap);
int func_08899300(void);
void func_088994C0(int arg);
void func_08899688(void);
void func_08899758(int task, int arg1, int arg2);

/* ScreenB41CC0: sets up the network session. */
void func_08B3C554(ScreenB41CC0 *self, int arg1, int arg2)
{
    void *heap;
    int object;

    self->field_C4->field_CC = 1;
    object = D_089D6AC8;
    heap = func_08B42B40();
    func_0889384C(object, heap);
    object = func_08899300();
    heap = func_08B42B40();
    func_08898DF8(object, heap);
    func_08899758(self->task.field_00, 1, 2000);
    func_08893B90(self->task.field_00, 1, 20000, -1, 60000, -1, 0x13114, 1, 0xE86C);
    func_08893C6C(self->task.field_00, 1, 2000);
    func_08893C94(D_089D6AC8, arg1);
    func_08893418(0, 0, 0, 1);
    func_088994C0(0);
    func_08892DCC(D_089D6AC8);
    func_08893534(arg2);
    func_08899688();
    D_09AFB89C = 0;
    func_088421EC();
}
