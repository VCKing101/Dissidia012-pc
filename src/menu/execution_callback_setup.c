#include "menu/execution_callbacks.h"

extern int func_088650A4(MainCallbackBlock *);

void func_089DE910(MenuRuntimePrefix *self, unsigned char flag) {
    D_09B6C2C0 = flag;
    func_0886503C(&self->callback_0100,func_089DEA14,func_089DEA34,0);
}

void func_089DE94C(MenuRuntimePrefix *self) {
    MainCallbackBlock *block = &self->callback_0100;
    func_088650A4(block);
    func_0886504C(block,(unsigned int)self,0x100);
}

