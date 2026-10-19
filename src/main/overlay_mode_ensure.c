#include "main/callback_runtime.h"

/* Only request a transition when the manager's signed mode byte differs. */
void func_08933C00(MainOverlayModePrefix *self) {
    if (self->mode != 0) func_08933ACC(self, 0);
}
void func_08933C28(MainOverlayModePrefix *self) {
    if (self->mode != 1) func_08933ACC(self, 1);
}
void func_08933C54(MainOverlayModePrefix *self) {
    if (self->mode != 2) func_08933ACC(self, 2);
}
