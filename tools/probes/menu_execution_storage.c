#include "menu/execution_state.h"
extern const char D_089E076C[];
extern void *func_0888204C(unsigned int);
extern void *func_0885C40C(void *,unsigned int,const char *,unsigned int);
extern void func_0885C488(void *);
extern void func_0888208C(void *);
void func_089DE870(MenuRuntimePrefix *self) {
    unsigned int size = 0x270800;
    self->field_0134 = func_0888204C(size);
    self->field_0138 = func_0885C40C(self->field_0134,size,D_089E076C,0x40);
}
void func_089DE8C8(MenuRuntimePrefix *self) {
    func_0885C488(self->field_0138);
    void *buffer = self->field_0134;
    self->field_0138 = 0;
    func_0888208C(buffer);
    self->field_0134 = 0;
}
void func_089DE900(MenuRuntimePrefix *self, unsigned int unused_first, unsigned int unused_second) {self->field_01DC->field_004A = 1;}
