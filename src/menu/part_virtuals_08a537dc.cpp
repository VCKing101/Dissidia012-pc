#include "menu/agent6_types.h"

struct A6PartList2 {
    char pad_000[0x228];
    A6Part *part[8];
};

extern "C" {
void func_08A537DC(A6PartList2 *self, int i) {
    self->part[i]->v1();
}

void func_08A53814(A6PartList2 *self, int i) {
    self->part[i]->v2();
}

void func_08A5384C(A6PartList2 *self, int i) {
    self->part[i]->v3();
}

void func_08A53884(A6PartList2 *self, int i) {
    self->part[i]->v4();
}

void func_08A538BC(A6PartList2 *self, int i) {
    self->part[i]->v5();
}
}
