/* Uncredited probe: four prologue words differ in scheduling. */
#include "main/resource_manager_commands.h"

int func_088A15B8(MainManagerSlotPrefix *self, unsigned int a, unsigned int b) {
    int index = 0;
    do {
        MainManagerSlotPrefix *slot = (MainManagerSlotPrefix *)((unsigned char *)self + index);
        if (slot->field_0068[0] == 0) {
            slot->field_0068[0] = 1;
            if (func_0899075C(a, b, index) == 1) return index;
            else return -1;
        }
    } while (++index < 20);
    return -1;
}
