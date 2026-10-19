#include "main/state_prefixes.h"

extern const unsigned char D_089C9370[];
extern unsigned int func_08882390(unsigned int, unsigned int, const void *);

void func_0886504C(MainStatePrefix *self, unsigned int a, unsigned int b) {
    self->field_0000 = func_08882390(a, b, D_089C9370);
}
