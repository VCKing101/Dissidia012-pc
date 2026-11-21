#include "menu/agent36_types.h"

/* List: while entries remain, starts the event name (kind 4) with its two
 * callbacks and steps the list index. */

extern "C" {
void func_088E9958(int arg);
void func_088E980C(void *name, int arg);
void func_088E991C(int arg, void (*done)(void));
void func_08A4303C(void);
void func_08A43094(void);
}

extern "C" void func_08A42FC0(void *name)
{
    ListState36 *state = &D_09B6DA68;

    if (state->count >= state->index) {
        Kept<void *> saved(name);
        Kept<ListState36 *> list(state);
        func_088E9958(0);
        func_088E980C(saved.value, 4);
        func_088E991C(0, func_08A4303C);
        func_088E991C(2, func_08A43094);
        list.value->index++;
    }
}
