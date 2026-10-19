#include "menu/execution_state.h"
extern MenuRuntimePrefix *D_089E09C0;
extern unsigned char D_09B6C2C0;
extern unsigned char D_089D5F20;
extern void func_089DE3AC(MenuRuntimePrefix *);
extern void func_089DE548(MenuRuntimePrefix *);
extern void func_089DD5DC(MenuRuntimePrefix *);
extern void func_089DE900(MenuRuntimePrefix *,unsigned int,unsigned int);
extern int func_088650A4(MainCallbackBlock *);
extern void func_0886243C(void *);
void func_089DEA14(void) {func_089DE3AC(D_089E09C0);}
void func_089DEA34(void) {func_089DE548(D_089E09C0);}
void func_089DEA54(void) {func_089DD5DC(D_089E09C0);}
void func_089DE910(MenuRuntimePrefix *self, unsigned char flag) {
    D_09B6C2C0 = flag;
    func_0886503C(&self->callback_0100,func_089DEA14,func_089DEA34,0);
}
void func_089DE94C(MenuRuntimePrefix *self) {
    MainCallbackBlock *block = &self->callback_0100;
    func_088650A4(block);
    func_0886504C(block,(unsigned int)self,0x100);
}
void func_089DE990(MenuRuntimePrefix *self) {
    self->field_01DC->field_0008 = 19;
    func_08865158(&self->callback_0100);
    if (!D_089D5F20) func_0886243C(self->unknown_01E0);
}
void func_089DE9EC(unsigned int first,unsigned int second) {
    func_089DE900(D_089E09C0,first,second);
}
void func_089DEA74(void) {
    func_089DE910(D_089E09C0,0);
    func_089DE94C(D_089E09C0);
}
