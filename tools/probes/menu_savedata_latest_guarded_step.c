#include "menu/execution_state.h"
extern const unsigned long long D_089E0738;
extern int func_089C5D78(const void *, unsigned long long *);
extern void *func_08935BB4(void *, const void *, unsigned int);
extern int func_089C5858(MenuSavedataRequest *);
struct SlotTick {unsigned long long value;};
void func_089DD9DC(MenuRuntimePrefix *self) {
    struct SlotTick latest = {D_089E0738};
    int found = 0;
    {
        int i;
        for (i = 0; i < 20; ++i) {
            struct SlotTick current;
            if (!self->field_01DC->entries_0688[i].name[0]) continue;
            func_089C5D78(&self->field_01DC->entries_0688[i].modified,&current.value);
            if (current.value < latest.value) continue;
            func_08935BB4(self->field_01DC->savedata_007C.save_name,
                         self->field_01DC->entries_0688[i].name,20);
            self->field_01DC->cached_time_3FA98 = self->field_01DC->entries_0688[i].modified;
            latest = current;
            found = 1;
        }
    }
    if (!found) {
        self->field_01DC->field_0008 = 6;
        return;
    }
    self->field_01DC->savedata_007C.mode = 0;
    self->field_01DC->savedata_007C.multi_status = 0;
    int result = func_089C5858(&self->field_01DC->savedata_007C);
    MenuExecutionState *state = self->field_01DC;
    int next_step = 6;
    if (!result) next_step = state->field_0008 + 1;
    state->field_0008 = next_step;
}
