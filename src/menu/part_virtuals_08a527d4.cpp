#include "menu/agent6_types.h"

struct A6PartList {
    char pad_000[0x84];
    A6Part *part[8];
};

extern "C" {
void func_08A527D4(A6PartList *self, int i) {
    self->part[i]->v3();
}

void func_08A5280C(A6PartList *self, int i) {
    self->part[i]->v4();
}

void func_08A52844(A6PartList *self, int i) {
    self->part[i]->v5();
}
}
