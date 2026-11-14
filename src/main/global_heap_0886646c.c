#include "main/agent2_types.h"

extern Agent2GlobalState *D_089D5EC0;
extern int func_0899B9F0(int);
extern void func_0899BB68(int, int, int, int);
extern void *func_0885C6CC(void *, unsigned int, int, int, int, int);
extern void func_0885C538(void *, void *, int);

int func_0886646C(int first, int second) {
    int handle = func_0899B9F0(0);
    func_0899BB68(handle, first, second, 0);
    return handle;
}

void *func_088664C4(unsigned int size) {
    return func_0885C6CC(D_089D5EC0->field_0710, size, 0x10, 0, 1, 0);
}

void func_088664FC(void *memory) {
    func_0885C538(D_089D5EC0->field_0710, memory, 0x10);
}

void *func_08866528(unsigned int size) {
    int high = 0;
    return func_0885C6CC(D_089D5EC0->field_0714, size, 0, 0, high, 0);
}

void func_08866560(void *memory) {
    func_0885C538(D_089D5EC0->field_0714, memory, 0);
}

void *func_0886658C(unsigned int size) {
    return func_0885C6CC(D_089D5EC0->field_0710, size, 0x40, 0, 1, 0);
}

void func_088665C4(void *memory) {
    func_0885C538(D_089D5EC0->field_0710, memory, 0x40);
}
