typedef struct MenuSceneState {
    int field_00;
    int field_04;
} MenuSceneState;

extern void func_08A5287C(MenuSceneState *self);

void func_08A525B0(MenuSceneState *self) {
    if (self->field_04 == 1) {
        func_08A5287C(self);
    }
}
