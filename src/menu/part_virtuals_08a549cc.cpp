#include "menu/agent6_types.h"

struct A6PartList3 {
    char pad_000[0x58];
    A6Part *part[8];
};

extern "C" {
void func_08A549CC(A6PartList3 *self, int i) {
    self->part[i]->v1();
}

void func_08A54A04(A6PartList3 *self, int i) {
    self->part[i]->v2();
}

void func_08A54A3C(A6PartList3 *self, int i) {
    self->part[i]->v3();
}

void func_08A54A74(A6PartList3 *self, int i) {
    self->part[i]->v4();
}

void func_08A54AAC(A6PartList3 *self, int i) {
    self->part[i]->v5();
}
}
