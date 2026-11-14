#include "main/agent2_state_list.h"

extern "C" MainStatePrefix *func_088651AC(MainStatePrefix *state) {
    return state == 0 ? D_089D5EB0->field_0000
                      : Agent2StateList::following(state);
}

extern "C" void func_088651CC(MainStatePrefix *state) {
    D_089D5EB0->push(state);
}

extern "C" void func_08865204(MainStatePrefix *state) {
    D_089D5EB0->remove(state);
}

extern "C" void func_08865270(void) {
}
