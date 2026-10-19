#include "main/state_prefixes.h"

/* Initializer callers: main 0x0882F524 and 0x08894EA4.
 * Setter callers: main 0x0880FEC8, 0x088101D8, and 0x08830444.
 * All four getters are called by main 0x0886AB1C. */
void func_0886F074(MainFloatRecordPrefix *self) {
    self->field_0000 = -1;
    self->field_0004 = 0.0f;
    self->field_0008 = 0.0f;
}
void func_0886F08C(MainFloatRecordPrefix *self, float value) {
    self->field_0004 = value;
}
int func_0886F094(MainFloatRecordPrefix *self) {
    return self->field_0000;
}
float func_0886F09C(MainFloatRecordPrefix *self) {
    return self->field_0004;
}
float func_0886F0A4(MainFloatRecordPrefix *self) {
    return self->field_0008;
}
float func_0886F0AC(MainFloatRecordPrefix *self) {
    return self->field_000C;
}
