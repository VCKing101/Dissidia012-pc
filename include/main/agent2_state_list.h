#ifndef MAIN_AGENT2_STATE_LIST_H
#define MAIN_AGENT2_STATE_LIST_H
#include "main/state_prefixes.h"

/* Singly linked list of state blocks threaded through field_0004. The
 * list object at D_089D5EB0 holds the head and tail. */
extern "C" struct Agent2StateList *D_089D5EB0;

struct Agent2StateList {
    MainStatePrefix *field_0000;
    MainStatePrefix *field_0004;

    static MainStatePrefix *following(MainStatePrefix *state) {
        return (MainStatePrefix *)state->field_0004;
    }
    void push(MainStatePrefix *state) {
        if (field_0000 == 0) {
            field_0004 = state;
            D_089D5EB0->field_0000 = state;
            state->field_0004 = 0;
        } else {
            state->field_0004 = (unsigned int)field_0000;
            D_089D5EB0->field_0000 = state;
        }
    }
    void remove(MainStatePrefix *state) {
        if (field_0000 != 0 && field_0004 != 0) {
            MainStatePrefix *next = following(state);
            MainStatePrefix *previous;
            if (state == field_0000) {
                field_0000 = next;
                previous = 0;
            } else {
                previous = field_0000;
                while (following(previous) != state) {
                    previous = following(previous);
                }
                previous->field_0004 = (unsigned int)next;
            }
            if (state == field_0004) field_0004 = previous;
            state->field_0004 = 0;
        }
    }
};

#endif
