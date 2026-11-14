#include "main/agent2_types.h"

void func_08864C74(Agent2Point *self, float value) {
    self->field_0000 = value;
}

void func_08864C7C(Agent2Point *self, float value) {
    self->field_0004 = value;
}

void func_08864C84(Agent2Point *self, unsigned char red, unsigned char green,
                   unsigned char blue) {
    unsigned char alpha = 0xFF;
    self->field_0008 = (((((alpha << 8) + blue) << 8) + green) << 8) + red;
}

void func_08864CB0(Agent2Point *self, unsigned int color) {
    self->field_0008 = color;
}

float func_08864CB8(Agent2Point *self) {
    return self->field_0000;
}

float func_08864CC0(Agent2Point *self) {
    return self->field_0004;
}

unsigned int func_08864CC8(Agent2Point *self) {
    return self->field_0008;
}

float func_08864CD0(Agent2Point *self) {
    return self->field_0000;
}

void func_08864CD8(Agent2Point *self, float first, float second) {
    self->field_0008 |= 1;
    self->field_0000 = first;
    self->field_0004 = second;
}

void func_08864CF0(Agent2Point *self) {
    self->field_0008 &= ~1;
    self->field_0004 = 0.0f;
}
